/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

#include "gfx/legato/image/raw/legato_imagedecoder_raw.h"

#include "gfx/legato/asset/legato_palette.h"
#include "gfx/legato/common/legato_pixelbuffer.h"
#include "gfx/legato/image/legato_image.h"
#include "gfx/legato/image/legato_image_utils.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/renderer/legato_renderer.h"


#if LE_ASSET_STREAMING_ENABLED == 1

#if LE_ASSET_DECODER_USE_PALETTE_CACHE == 1
#define cache leRawImageDecoderPaletteScratchBuffer
#endif

static struct StreamPaletteStage
{
    leRawDecodeStage base;

#if LE_ASSET_DECODER_USE_PALETTE_CACHE == 1
    uint32_t cacheSize;
    uint32_t cacheIndexStart;
    uint32_t cacheIndexEnd;
    lePixelBuffer cacheBuffer;
#endif

    uint32_t paletteSize;

    leBool mediaOpen;

    leAssetStreamReader reader;
} streamPaletteStage;

#if LE_ASSET_DECODER_USE_PALETTE_CACHE == 1
static leResult requestImageData(leRawDecodeState* state)
{
    uint32_t index;
    uint32_t offs;
    void* address;
    uint32_t modeSize;
    uint32_t remaining;

    streamPaletteStage.cacheIndexStart = streamPaletteStage.base.state->sourceColor;

    offs = leGetOffsetFromIndexAndBPP(streamPaletteStage.cacheIndexStart,
                                      leColorModeInfoGet(state->source->palette->colorMode).bppOrdinal);

    address = state->source->palette->header.dataAddress;
    address = (void*)(((uint8_t*)address) + offs);

    modeSize = leColorModeInfoGet(state->source->palette->colorMode).size;

    // calculate how much data is left in the image
    remaining = state->source->palette->header.dataSize - offs;

    // make sure we stay within the width of the cache, aligned to the mode pixel size in bytes
    streamPaletteStage.cacheSize = (LE_ASSET_DECODER_CACHE_SIZE / modeSize);
    streamPaletteStage.cacheSize *= modeSize;

    // prevent buffer overrun
    if(streamPaletteStage.cacheSize >= remaining)
    {
        streamPaletteStage.cacheSize = remaining / modeSize;
        streamPaletteStage.cacheSize *= modeSize;
    }

    streamPaletteStage.cacheIndexEnd = streamPaletteStage.cacheIndexStart + (streamPaletteStage.cacheSize / modeSize) - 1;

    streamPaletteStage.reader.state = LE_STREAM_WAITING;

    return leApplication_MediaReadRequest(&streamPaletteStage.reader,
                                          address,
                                          streamPaletteStage.cacheSize,
                                          cache);
}
#endif

static void stage_lookup(struct StreamPaletteStage* stage)
{
    if(streamPaletteStage.reader.state == LE_STREAM_WAITING)
        return;

#if LE_ASSET_DECODER_USE_PALETTE_CACHE == 1
    if(streamPaletteStage.cacheIndexStart == streamPaletteStage.cacheIndexEnd ||
        (streamPaletteStage.base.state->sourceColor < streamPaletteStage.cacheIndexStart ||
         streamPaletteStage.base.state->sourceColor > streamPaletteStage.cacheIndexEnd))
    {
        requestImageData(stage->base.state);

        if(streamPaletteStage.reader.state == LE_STREAM_WAITING)
        {
            return;
        }
    }

    // get a pixel from the cache
    stage->base.state->sourceColor = lePixelBufferGet_Unsafe(&streamPaletteStage.cacheBuffer,
                                                             streamPaletteStage.base.state->sourceColor - streamPaletteStage.cacheIndexStart,
                                                             0);
#else
    leApplication_MediaReadRequest((leAssetStreamReader*)&streamPaletteStage.reader,
                                   (void*)((uint32_t)streamPaletteStage.base.state->source->palette->header.dataAddress +
                                       streamPaletteStage.base.state->sourceColor * stage->paletteSize),
                                   stage->paletteSize,
                                   (uint8_t*)&streamPaletteStage.base.state->sourceColor);
#endif

    stage->base.state->blendStage->exec(stage->base.state->blendStage);
}

static void exec(struct StreamPaletteStage* stage)
{
    if(leApplication_MediaOpenRequest((leAssetStreamReader*)stage->base.state) == LE_FAILURE)
        return;

    streamPaletteStage.mediaOpen = LE_TRUE;

    stage->base.exec = (void*)stage_lookup;

    stage_lookup(stage);
}

static void cleanup(struct StreamPaletteStage* stage)
{
    if(streamPaletteStage.mediaOpen == LE_TRUE)
    {
        leApplication_MediaCloseRequest((leAssetStreamReader *) stage->base.state);
    }
}

static void readerExec(leAssetStreamReader* reader)
{
    if(streamPaletteStage.reader.state == LE_STREAM_READY ||
        streamPaletteStage.reader.state == LE_STREAM_WAITING)
    {
        streamPaletteStage.base.exec((void*)&streamPaletteStage);
    }
}

static void readerDataReady(leAssetStreamReader* reader)
{
    if(streamPaletteStage.reader.state == LE_STREAM_WAITING)
    {
        streamPaletteStage.reader.state = LE_STREAM_DATAREADY;
    }
}

static void readerAbort(leAssetStreamReader* reader)
{
    streamPaletteStage.reader.state = LE_STREAM_ABORT;
}

void _leRawImageDecoder_PaletteStreamInit(leRawDecodeState* state)
{
    memset(&streamPaletteStage, 0, sizeof(streamPaletteStage));

    streamPaletteStage.reader.asset = (leAssetHeader*)state->source->palette;
    streamPaletteStage.reader.exec = (void*)readerExec;
    streamPaletteStage.reader.dataReady = (void*)readerDataReady;
    streamPaletteStage.reader.abort = (void*)readerAbort;
    streamPaletteStage.reader.status = LE_READER_STATUS_READY;
    streamPaletteStage.reader.state = LE_STREAM_READY;
    streamPaletteStage.reader.result = 0;
    streamPaletteStage.reader.userData = NULL;
    streamPaletteStage.reader.onFinished = NULL;

    streamPaletteStage.base.state = state;
    streamPaletteStage.base.exec = (void*)exec;
    streamPaletteStage.base.cleanup = (void*)cleanup;

#if LE_ASSET_DECODER_USE_PALETTE_CACHE == 1
    lePixelBufferCreate(LE_ASSET_DECODER_CACHE_SIZE / leColorModeInfoGet(state->source->palette->colorMode).size,
                        1,
                        state->source->palette->colorMode,
                        cache,
                        &streamPaletteStage.cacheBuffer);
#endif

    streamPaletteStage.paletteSize = leColorModeInfoGet(state->source->palette->colorMode).size;

    streamPaletteStage.base.exec = (void*)stage_lookup;

    state->paletteStage = (void*)&streamPaletteStage;

    if(state->maskStage == NULL)
    {
        state->maskStage = state->paletteStage;
    }
}

#endif