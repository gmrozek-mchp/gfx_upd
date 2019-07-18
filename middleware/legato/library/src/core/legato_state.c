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

#include "gfx/legato/core/legato_state.h"

#include "gfx/legato/core/legato_input.h"

#include "gfx/legato/image/legato_image.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/renderer/legato_renderer.h"


#include <string.h>

int32_t _initialized = LE_FALSE;

#define LE_MAX_BUFFER_COUNT 2

static leState _state;

leState* leGetState()
{
    return &_state; 
}

/* vtable generation functions, make sure child classes come after base ones */
typedef void (*vtableFn)();

void _leString_GenerateVTable();
void _leDynamicString_GenerateVTable();
void _leFixedString_GenerateVTable();
void _leTableString_GenerateVTable();

void _leWidget_GenerateVTable();
void _leEditWidget_GenerateVTable();

void _leArcWidget_GenerateVTable();
void _leBarGraphWidget_GenerateVTable();
void _leButtonWidget_GenerateVTable();
void _leCheckBoxWidget_GenerateVTable();
void _leCircleWidget_GenerateVTable();
void _leCircularGuageWidget_GenerateVTable();
void _leCircularSliderWidget_GenerateVTable();
void _leDrawSurfaceWidget_GenerateVTable();
void _leGradientWidget_GenerateVTable();
void _leGroupBoxWidget_GenerateVTable();
void _leImageWidget_GenerateVTable();
void _leImagePlusWidget_GenerateVTable();
void _leImageSequenceWidget_GenerateVTable();
void _leKeyPadWidget_GenerateVTable();
void _leLabelWidget_GenerateVTable();
void _leLineWidget_GenerateVTable();
void _leLineGraphWidget_GenerateVTable();
void _leListWidget_GenerateVTable();
void _leListWheelWidget_GenerateVTable();
void _lePieChartWidget_GenerateVTable();
void _leProgressBarWidget_GenerateVTable();
void _leRadialMenuWidget_GenerateVTable();
void _leRadioButtonWidget_GenerateVTable();
void _leRectangleWidget_GenerateVTable();
void _leScrollBarWidget_GenerateVTable();
void _leSliderWidget_GenerateVTable();
void _leTextFieldWidget_GenerateVTable();
void _leTouchTestWidget_GenerateVTable();
void _leWindowWidget_GenerateVTable();

vtableFn vtableFnTable[] =
{
    _leString_GenerateVTable,
    _leDynamicString_GenerateVTable,
    _leFixedString_GenerateVTable,
    _leTableString_GenerateVTable,
    
    _leWidget_GenerateVTable,
    _leEditWidget_GenerateVTable,

#if LE_ARC_WIDGET_ENABLED == 1
    _leArcWidget_GenerateVTable,
#endif

#if LE_BARGRAPH_WIDGET_ENABLED == 1
    _leBarGraphWidget_GenerateVTable,
#endif

#if LE_BUTTON_WIDGET_ENABLED == 1
    _leButtonWidget_GenerateVTable,
#endif

#if LE_CHECKBOX_WIDGET_ENABLED == 1
    _leCheckBoxWidget_GenerateVTable,
#endif

#if LE_CIRCLE_WIDGET_ENABLED == 1
    _leCircleWidget_GenerateVTable,
#endif

#if LE_CIRCULARGUAGE_WIDGET_ENABLED == 1
    _leCircularGuageWidget_GenerateVTable,
#endif

#if LE_CIRCULARSLIDER_WIDGET_ENABLED == 1
    _leCircularSliderWidget_GenerateVTable,
#endif

#if LE_DRAWSURFACE_WIDGET_ENABLED == 1
    _leDrawSurfaceWidget_GenerateVTable,
#endif

#if LE_GRADIENT_WIDGET_ENABLED == 1
    _leGradientWidget_GenerateVTable,
#endif

#if LE_GROUPBOX_WIDGET_ENABLED == 1
    _leGroupBoxWidget_GenerateVTable,
#endif

#if LE_IMAGE_WIDGET_ENABLED == 1
    _leImageWidget_GenerateVTable,
#endif

#if LE_IMAGEPLUS_WIDGET_ENABLED == 1
    _leImagePlusWidget_GenerateVTable,
#endif

#if LE_IMAGESEQUENCE_WIDGET_ENABLED == 1
    _leImageSequenceWidget_GenerateVTable,
#endif

#if LE_KEYPAD_WIDGET_ENABLED == 1
    _leKeyPadWidget_GenerateVTable,
#endif

#if LE_LABEL_WIDGET_ENABLED == 1
    _leLabelWidget_GenerateVTable,
#endif

#if LE_LINE_WIDGET_ENABLED == 1
    _leLineWidget_GenerateVTable,
#endif

#if LE_LINEGRAPH_WIDGET_ENABLED == 1
    _leLineGraphWidget_GenerateVTable,
#endif

#if LE_LIST_WIDGET_ENABLED == 1
    _leListWidget_GenerateVTable,
#endif

#if LE_LISTWHEEL_WIDGET_ENABLED == 1
    _leListWheelWidget_GenerateVTable,
#endif

#if LE_PIECHART_WIDGET_ENABLED == 1
    _lePieChartWidget_GenerateVTable,
#endif

#if LE_PROGRESSBAR_WIDGET_ENABLED == 1
    _leProgressBarWidget_GenerateVTable,
#endif

#if LE_RADIALMENU_WIDGET_ENABLED == 1
    _leRadialMenuWidget_GenerateVTable,
#endif

#if LE_RADIOBUTTON_WIDGET_ENABLED == 1
    _leRadioButtonWidget_GenerateVTable,
#endif

#if LE_RECTANGLE_WIDGET_ENABLED == 1
    _leRectangleWidget_GenerateVTable,
#endif

#if LE_SCROLLBAR_WIDGET_ENABLED == 1
    _leScrollBarWidget_GenerateVTable,
#endif

#if LE_SLIDER_WIDGET_ENABLED == 1
    _leSliderWidget_GenerateVTable,
#endif

#if LE_TEXTFIELD_WIDGET_ENABLED == 1
    _leTextFieldWidget_GenerateVTable,
#endif

#if LE_TOUCHTEST_WIDGET_ENABLED == 1
    _leTouchTestWidget_GenerateVTable,
#endif

#if LE_WINDOW_WIDGET_ENABLED == 1
    _leWindowWidget_GenerateVTable,
#endif

    NULL
};

leResult leInitialize(const leDisplayDriver* dispDriver)
{
    uint32_t idx;
    leWidget* root;
    
    if(_initialized == LE_TRUE)
        return LE_FAILURE;
        
    memset(&_state, 0, sizeof(leState));
        
    if(leMemory_Init() == LE_FAILURE ||
       leEvent_Init() == LE_FAILURE ||
       leInput_Init() == LE_FAILURE ||
       leRenderer_Initialize(dispDriver) == LE_FAILURE)
    {
        return LE_FAILURE;
    }
    
    /* initialize virtual function tables */
    idx = 0;
    
    while(vtableFnTable[idx] != NULL)
    {
        vtableFnTable[idx]();
        
        idx += 1;
    }
    
    leImage_InitDecoders();
    leScheme_Initialize(&_state.defaultScheme, dispDriver->getColorMode());

    for(idx = 0; idx < LE_LAYER_COUNT; idx++)
    {
        root = &_state.rootWidget[idx];
        
        leWidget_Constructor(root);
        
        root->fn->setPosition(root, 0, 0);
        
        root->fn->setSize(root,
                          dispDriver->getDisplayWidth(),
                          dispDriver->getDisplayHeight());
    }
    
    

    //_state.activeScreen = -1;

    _initialized = LE_TRUE;

    return LE_SUCCESS;
}

void leShutdown()
{
    uint32_t idx;
    leWidget* root;
    
    if(_initialized == LE_FALSE)
        return;
    
    for(idx = 0; idx < LE_LAYER_COUNT; idx++)
    {
        root = &_state.rootWidget[idx];
        
        root->fn->_destructor(root);
    }
        
    /*for(idx = 0; idx < LE_MAX_SCREENS; idx++)
    {
        if(_state.screens[idx].used == LE_TRUE)
        {
            if(idx == _state.activeScreen)
            {
                _state.screens[idx].intf.hide(&_state.screens[idx].rootWidget);
            
                _state.activeScreen = -1;
            }
            
            _state.screens[idx].intf.destroy(&_state.screens[idx].rootWidget);
            
            _state.screens[idx].rootWidget.fn->destructor(&_state.screens[idx].rootWidget);
            
            _state.screens[idx].used = LE_FALSE;
        }
    }*/
    
    leRenderer_Shutdown();
    leInput_Shutdown();
    leEvent_Shutdown();
    
    memset(&_state, 0, sizeof(leState));

    _initialized = LE_FALSE;
}

static uint32_t updateWidgets(uint32_t dt)
{
#if 0
    leScreen* activeScreen;
    leLayer* layer;
    uint32_t modDelta;
    int32_t i;
    leWidgetUpdateState widgetState;
    leContextUpdateState state = LE_CONTEXT_UPDATE_DONE;;

    activeScreen = leGetActive()->activeScreen;
    
    if(activeScreen == NULL)
        return LE_CONTEXT_UPDATE_DONE;

    // iterate over all existing layers for update
    for(i = 0; i < LE_MAX_LAYERS; i++)
    {
        layer = activeScreen->layers[i];

        if(layer == NULL)
            continue;
            
        if(layer->frameState <= LE_LAYER_FRAME_PREFRAME)
        {
            modDelta = dt;
            
            if(layer->deltaTime != 0)
            {
                modDelta += layer->deltaTime;
                layer->deltaTime = 0;
            }
            
            widgetState = _leUpdateWidget((leWidget*)layer, modDelta);
            if (widgetState != LE_WIDGET_UPDATE_STATE_DONE)
                state = LE_CONTEXT_UPDATE_PENDING;
        }
        else
        {
            layer->deltaTime += dt;
        }
    }

    return state;
#else
    return 0;
#endif
}

leResult leUpdate(uint32_t dt)
{
    leEvent_ProcessEvents();
    
    updateWidgets(dt);
    
    leRenderer_Paint();
    
    return LE_SUCCESS;
}

leColorMode leGetColorMode()
{
    return leGetRenderState()->colorMode;
}

leRect leGetDisplayRect()
{
    return leGetRenderState()->displayRect;
}

leStringTable* leGetStringTable()
{
    return (leStringTable*)_state.stringTable;
}

void leSetStringTable(const leStringTable* table)
{
    _state.stringTable = table;    
    
    leRedrawAll();
}

uint32_t leGetStringLanguage()
{        
    return _state.languageID;
}

/*static void signalLanguageChanging(leWidget* wgt)
{
    uint32_t i;
    leWidget* child;
    
    // signal event
    wgt->fn->languageChangeEvent(wgt);
    
    for(i = 0; i < wgt->children.size; i++)
    {
        child = wgt->children.values[i];

        signalLanguageChanging(child);
    }
}*/

void leSetStringLanguage(uint32_t id)
{   
#if 0
    leScreen* activeScreen;
    leRect screenRect;
    leLayer* layer;
    uint32_t i;
    
    if(_leState.languageID == id)
        return;
        
    activeScreen = leGetActive()->activeScreen;
    
    screenRect = leGetScreenRect();
    
    if(activeScreen == NULL)
        return;
        
    // iterate over all qualifying layers and signal event
    for(i = 0; i < LE_MAX_LAYERS; i++)
    {
        layer = activeScreen->layers[i];

        if(layer == NULL ||
           layer->widget.enabled == LE_FALSE ||
           leRectIntersects(&screenRect, &layer->widget.rect) == LE_FALSE)
            continue;
            
        signalLanguageChanging((leWidget*)layer);
    }
    
    _activeContext->languageID = id;

	// do it again after the change
    for(i = 0; i < LE_MAX_LAYERS; i++)
    {
        layer = activeScreen->layers[i];

        if(layer == NULL ||
           layer->widget.enabled == LE_FALSE ||
           leRectIntersects(&screenRect, &layer->widget.rect) == LE_FALSE)
            continue;
            
        signalLanguageChanging((leWidget*)layer);
    }
#endif
}

void leRedrawAll()
{
#if 0
    leScreen* activeScreen;
    leLayer* layer;
    leRect screenRect;
    uint32_t i;
    
    if(_activeContext == NULL)
        return;
        
    activeScreen = leGetActive()->activeScreen;
    
    if(activeScreen == NULL)
        return;
        
    screenRect = leGetScreenRect();
        
    // iterate over all qualifying layers and invalidate
    for(i = 0; i < LE_MAX_LAYERS; i++)
    {
        layer = activeScreen->layers[i];

        if(layer == NULL ||
           layer->widget.enabled == LE_FALSE ||
           leRectIntersects(&screenRect, &layer->widget.rect) == LE_FALSE)
        {
            continue;
        }
        
        leWidget_Invalidate((leWidget*)layer);
    }
#endif
}

leScheme* leGetDefaultScheme()
{
    return &_state.defaultScheme;
}

leWidget* leGetFocusWidget()
{
    return _state.focus;
}

leResult leSetFocusWidget(leWidget* widget)
{
    leEvent evt;
    
    if(_state.focus == widget)
        return LE_SUCCESS;

    if(_state.focus != NULL)
    {
        evt.id = LE_WIDGET_EVENT_FOCUS_LOST;
        
        _state.focus->fn->_handleEvent(_state.focus, &evt);
    } 

    _state.focus = widget;

    if(_state.focus != NULL)
    {
        evt.id = LE_WIDGET_EVENT_FOCUS_GAINED;
        
        _state.focus->fn->_handleEvent(_state.focus, &evt);
    }
    
    return LE_SUCCESS;
}

leEditWidget* leGetEditWidget()
{
    return _state.edit;
}

leResult leSetEditWidget(leEditWidget* widget)
{
    if(_state.edit == (leEditWidget*)widget)
        return LE_SUCCESS;

    if(_state.edit != NULL)
    {
        _state.edit->fn->editEnd(_state.edit);
    }

    _state.edit = widget;

    if(_state.edit != NULL && _state.edit->fn->editStart(_state.edit) == LE_FAILURE)
    {
        _state.edit = NULL;
        
        return LE_FAILURE;
    }

    return LE_SUCCESS;
}

leBool leIsDrawing()
{
    return leGetRenderState()->frameState > LE_FRAME_PREFRAME;
}

#if 0
leResult leSetScreen(leScreenIntf intf, uint32_t idx)
{
    if(_state.screens[idx].used == LE_TRUE)
        return LE_FAILURE;
    
    leWidget_Constructor(&_state.screens[idx].rootWidget);
    
    _state.screens[idx].intf = intf;
    _state.screens[idx].used = LE_TRUE;
    _state.screens[idx].intf.initialize(&_state.screens[idx].rootWidget);
        
    return LE_SUCCESS;
}

leResult leClearScreen(uint32_t idx)
{
    if(idx == (uint32_t)_state.activeScreen)
        return LE_FAILURE;
        
    if(_state.screens[idx].used == LE_FALSE)
        return LE_FAILURE;
        
    _state.screens[idx].intf.destroy(&_state.screens[idx].rootWidget);
    _state.screens[idx].used = LE_FALSE;
    _state.screens[idx].rootWidget.fn->destructor(&_state.screens[idx].rootWidget);
    
    return LE_SUCCESS;
}

leWidget* leGetActiveScreenRoot()
{
    if(_state.activeScreen == -1)
        return NULL;
        
    return &_state.screens[_state.activeScreen].rootWidget;
}

leWidget* leGetScreenRoot(uint32_t idx)
{
    if(_state.screens[idx].used == LE_FALSE)
        return NULL;
        
    return &_state.screens[idx].rootWidget;
}

leResult leShowScreen(uint32_t idx)
{
    if(idx == (uint32_t)_state.activeScreen)
        return LE_SUCCESS;
        
    if(_state.screens[idx].used == LE_FALSE)
        return LE_FAILURE;
        
    if(_state.activeScreen != -1)
    {
        _state.screens[_state.activeScreen].intf.hide(&_state.screens[_state.activeScreen].rootWidget);
        _state.activeScreen = -1;
    }
    
    _state.activeScreen = idx;
    _state.screens[idx].intf.show(&_state.screens[idx].rootWidget);
}
#endif
/*
uint32_t leGetRootWidgetCount(uint32_t reserved)
{
    return _state.rootWidgets.size;
}

leWidget* leGetRootWidget(uint32_t idx, uint32_t reserved)
{
    if(idx >= _state.rootWidgets.size)
        return NULL;
    
    return _state.rootWidgets.values[idx];
}*/

leResult leAddRootWidget(leWidget* wgt,
                         uint32_t layer)
{
    if(wgt == NULL || layer > LE_LAYER_COUNT - 1)
        return LE_FAILURE;
        
    leRenderer_DamageArea(&wgt->rect);
        
    return _state.rootWidget[layer].fn->addChild(&_state.rootWidget[layer], wgt);
        
    /*leArray_PushBack(&_state.rootWidgets, wgt);
    
    if(wgt->visible == LE_TRUE)
    {
        leRenderer_DamageArea(&wgt->rect);
    }*/
        
    //return LE_SUCCESS;
}

leResult leRemoveRootWidget(leWidget* wgt, uint32_t layer)
{
    leRect rect;
    
    if(wgt == NULL)
        return LE_FAILURE;
        
    rect = wgt->rect;
        
    if(_state.rootWidget[layer].fn->removeChild(&_state.rootWidget[layer], wgt) == LE_SUCCESS)
    {
        leRenderer_DamageArea(&rect);
        
        return LE_SUCCESS;
    }
    
    return LE_FAILURE;
    
    /*if(leArray_Remove(&_state.rootWidgets, wgt) == LE_FAILURE)
        return LE_FAILURE;*/
        
    /*if(wgt->visible == LE_TRUE)
    {
        leRenderer_DamageArea(&wgt->rect);
    }*/
    
    //return LE_SUCCESS;
}

leBool leWidgetIsInScene(const leWidget* wgt)
{
    uint32_t layer;
    leWidget* root;
    leWidget* wgtRoot;
    
    if(wgt == NULL)
        return LE_FALSE;
        
    wgtRoot = wgt->fn->getRootWidget(wgt);
    
    if(wgtRoot == NULL)
        return LE_FALSE;
    
    for(layer = 0; layer < LE_LAYER_COUNT; layer++)
    {
        root = &_state.rootWidget[layer];
        
        if(root == wgtRoot)
            return LE_TRUE;
    }
    
    return LE_FALSE;
}

#if LE_EXTERNAL_STREAMING_ENABLED == 1
leExternalAssetReader* leGetReader()
{
    return _state.assetReader;
}

void leFreeReader()
{
    if(_state.assetReader != NULL)
    {
        LE_FREE(_state.assetReader );
        
        _state.assetReader = NULL;
    }
}

leResult leFreeReader()
{
    if(_state.assetReader == NULL)
        return LE_FAILURE;
    
    _state.assetReader->run(_state.assetReader);
    
    return LE_SUCCESS;
}
#endif

leResult leEdit_StartEdit()
{
    leEditWidget* edit = leGetEditWidget();
    
    if(edit == NULL)
        return LE_FAILURE;
        
    return edit->fn->editStart(edit);
}

void leEdit_EndEdit()
{ 
    leEditWidget* edit = leGetEditWidget();
    
    if(edit == NULL)
        return;
        
    edit->fn->editEnd(edit);
}

void leEdit_Clear()
{ 
    leEditWidget* edit = leGetEditWidget();
    
    if(edit == NULL)
        return;
        
    edit->fn->editClear(edit);
}

void leEdit_Accept()
{
    leEditWidget* edit = leGetEditWidget();
    
    if(edit == NULL)
        return;
        
    edit->fn->editAccept(edit);
}

void leEdit_Set(leString* str)
{
    leEditWidget* edit = leGetEditWidget();
    
    if(edit == NULL)
        return;
        
    edit->fn->editSet(edit, str);
}

void leEdit_Append(leString* str)
{
    leEditWidget* edit = leGetEditWidget();
    
    if(edit == NULL)
        return;
        
    edit->fn->editAppend(edit, str);
}

void leEdit_Backspace()
{
    leEditWidget* edit = leGetEditWidget();
    
    if(edit == NULL)
        return;
        
    edit->fn->editBackspace(edit);
}