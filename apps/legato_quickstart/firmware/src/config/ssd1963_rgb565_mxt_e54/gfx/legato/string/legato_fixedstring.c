#include "gfx/legato/string/legato_fixedstring.h"

#include <string.h>

#include "gfx/legato/common/legato_error.h"
#include "gfx/legato/memory/legato_memory.h"

static leFixedStringVTable fixedStringVTable;

void _leString_Constructor(leString* str);
void _leString_Destructor(leString* str);

void leFixedString_Constructor(leFixedString* _this,
                               leChar* buf,
                               uint32_t size)
{
    LE_ASSERT_THIS();
        
    _this->base.fn = (void*)&fixedStringVTable;
    _this->fn = (void*)&fixedStringVTable;
    
    _leString_Constructor((leString*)_this);
    
    _this->data = buf;
    _this->capacity = size;
    _this->length = 0;
    _this->data[0] = 0;
    _this->font = NULL;
}

leFixedString* leFixedString_New(leChar* buf,
                                 uint32_t size)
{
    leFixedString* str;
    
    str = LE_MALLOC(sizeof(leFixedString));
    
    leFixedString_Constructor(str, buf, size);
    
    return str;
}

void _leFixedString_Destructor(leFixedString* _this)
{
    LE_ASSERT_THIS();

    _this->data = NULL;
    _this->capacity = 0;
    _this->length = 0;
    _this->font = NULL;
    
    _leString_Destructor((leString*)_this);
}

leFont* _leFixedString_GetFont(const leFixedString* _this)
{
    LE_ASSERT_THIS();
        
    return (leFont*)_this->font;
}

leResult _leFixedString_SetFont(leFixedString* _this,
                                const leFont* font)
{
    LE_ASSERT_THIS();

    _this->fn->preinvalidate(_this);
        
    _this->font = font;

    _this->fn->invalidate(_this);
    
    return LE_SUCCESS;
}

leResult _leFixedString_SetFromString(leFixedString* _this,
                                      const leString* src)
{
    uint32_t srcLength, itr;
    
    LE_ASSERT_THIS();
    
    if(_this->data == NULL ||
       _this->capacity == 0 ||
       src == NULL)
    {
        return LE_FAILURE;
    }
    
    srcLength = src->fn->length(src);
    
    if(srcLength == 0)
        return LE_SUCCESS;
        
    if(_this->capacity < srcLength)
    {
        srcLength = _this->capacity;
    }

    _this->fn->preinvalidate(_this);
    
    _this->length = srcLength;
    
    for(itr = 0; itr < srcLength; itr++)
    {
        _this->data[itr] = src->fn->charAt(src, itr);
    }

    _this->fn->invalidate(_this);
    
    return LE_SUCCESS;
}

leResult _leFixedString_SetFromChar(leFixedString* _this,
                                    const leChar* buf,
                                    uint32_t size)
{
    uint32_t itr;
    
    LE_ASSERT_THIS();
    
    if(_this->data == NULL || _this->capacity == 0 || buf == NULL)
    {
        return LE_FAILURE;
    }
    
    if(size == 0)
        return LE_SUCCESS;
        
    if(_this->capacity < size)
    {
        size = _this->capacity;
    }

    _this->fn->preinvalidate(_this);

    _this->length = size;
    
    for(itr = 0; itr < size; itr++)
    {
        _this->data[itr] = buf[itr];
    }

    _this->fn->invalidate(_this);

    return LE_SUCCESS;
}

leResult _leFixedString_SetFromCStr(leFixedString* _this,
                                    const char* cstr)
{
    uint32_t len, itr;
    
    LE_ASSERT_THIS();
    
    if(_this->data == NULL || _this->capacity == 0 || cstr == NULL)
        return LE_FAILURE;
    
    len = strlen(cstr);
    
    if(len == 0)
        return LE_SUCCESS;
        
    if(_this->capacity < len)
    {
        len = _this->capacity;
    }

    _this->fn->preinvalidate(_this);
    
    _this->length = len;
    
    for(itr = 0; itr < len; itr++)
    {
        _this->data[itr] = (leChar)((unsigned char)cstr[itr]);
    }

    _this->fn->invalidate(_this);
    
    return LE_SUCCESS;
}

uint32_t _leFixedString_Length(const leFixedString* _this)
{
    LE_ASSERT_THIS();
        
    return _this->length;
}

leBool _leFixedString_IsEmpty(const leFixedString* _this)
{
    LE_ASSERT_THIS();
        
    return _this->length == 0;
}

leChar _leFixedString_CharAt(const leFixedString* _this,
                             uint32_t idx)
{
    LE_ASSERT_THIS();

    if(idx >= _this->length)
        return 0;
        
    return _this->data[idx];
}

int32_t _leFixedString_Compare(const leFixedString* _this,
                               const leString* tgt)
{
    uint32_t len, itr, chr;
    
    LE_ASSERT_THIS();
    
    if(_this == NULL && tgt == NULL)
        return 0;
        
    if((_this == NULL && tgt != NULL) || (_this != NULL && tgt == NULL))
        return -1;
    
    len = tgt->fn->length(tgt);
    
    if(_this->length < len)
    {
        len = _this->length;
    }
    
    if(len == 0)
        return 0;
        
    for(itr = 0; itr < len; itr++)
    {
        chr = tgt->fn->charAt(tgt, itr);
        
        if(_this->data[itr] != chr)
        {
            return _this->data[itr] - chr;
        }
    }
    
    return 0;
}

leResult _leFixedString_Append(leFixedString* _this,
                               const leString* val)
{
    uint32_t cpyLen, itr;
    
    LE_ASSERT_THIS();
    
    if(_this->data == NULL)
        return LE_FAILURE;
        
    if(val == NULL || val->fn->length(val) == 0)
        return LE_SUCCESS;
        
    cpyLen = val->fn->length(val);
    
    if(cpyLen >= _this->length)
    {
        cpyLen = _this->capacity - _this->length;
    }

    _this->fn->preinvalidate(_this);
    
    for(itr = 0; itr < cpyLen; itr++)
    {
        _this->data[_this->length + itr] = val->fn->charAt(val, itr);
    }

    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

leResult _leFixedString_Insert(leFixedString* _this,
                               const leString* val,
                               uint32_t idx)
{
    uint32_t cpyLen;
    int32_t itr;
    
    LE_ASSERT_THIS();
    
    if(_this->data == NULL)
        return LE_FAILURE;
        
    if(idx >= _this->length)
    {
        return _this->fn->append(_this, val);
    }
        
    if(val == NULL || val->fn->length(val) == 0)
        return LE_SUCCESS;
        
    cpyLen = val->fn->length(val);
    
    if(cpyLen == 0)
        return LE_SUCCESS;

    _this->fn->preinvalidate(_this);
    
    /* shift data right as much as possible to make room for the new values */
    for(itr = (int32_t)_this->length - 1; itr >= (int32_t)_this->length - 1 - (int32_t)idx; itr--)
    {
        if(itr + cpyLen < _this->capacity)
        {
            _this->data[itr + cpyLen] = _this->data[itr];
        }
    }
    
    /* insert new data */
    for(itr = 0; itr < (int32_t)cpyLen; itr++)
    {
        if(idx + itr >= _this->capacity)
            break;
            
        _this->data[idx + itr] = val->fn->charAt(val, itr); 
    }
    
    _this->length += cpyLen;
    
    if(_this->length > _this->capacity)
    {
        _this->length = _this->capacity;
    }

    _this->fn->invalidate(_this);
   
    return LE_SUCCESS;
}

leResult _leFixedString_Remove(leFixedString* _this,
                               uint32_t idx,
                               uint32_t count)
{
    uint32_t itr;
    
    LE_ASSERT_THIS();
    
    if(_this->data == NULL || idx >= _this->length)
        return LE_FAILURE;
    
    if(count == 0)
        return LE_SUCCESS;

    _this->fn->preinvalidate(_this);
    
    /* simple case, just move length index */
    if(idx + count == (uint32_t)_this->length - 1)
    {
        _this->length = idx;
        
        return LE_SUCCESS;
    }
    
    if(idx + count >= (uint32_t)_this->length)
    {
        count = _this->length - 1 - idx;
    }
    
    /* shift data left */
    for(itr = 0; itr < count; itr++)
    {
        _this->data[idx + itr] = _this->data[idx + itr + 1];
    }
       
    _this->length -= count;

    _this->fn->invalidate(_this);
    
    return LE_SUCCESS;
}

void _leFixedString_Clear(leFixedString* _this)
{
    LE_ASSERT_THIS();

    _this->fn->preinvalidate(_this);
    
    _this->length = 0;

    _this->fn->invalidate(_this);
}

uint32_t _leFixedString_ToChar(const leFixedString* _this,
                               leChar* buf,
                               uint32_t size)
{
    LE_ASSERT_THIS();
    
    if(_this->data == NULL || buf == NULL || size == 0)
        return LE_FAILURE;
        
    if(size >= _this->length)
    {
        size = _this->length;
    }
    
    memcpy(buf, _this->data, size * sizeof(leChar));
    
    return size;
}

void _leString_FillVTable(leStringVTable* vt);

void _leFixedString_GenerateVTable()
{
    _leString_FillVTable((void*)&fixedStringVTable);
    
    // override
    fixedStringVTable.destructor = _leFixedString_Destructor;
    
    // local
    fixedStringVTable.getFont = _leFixedString_GetFont;
    fixedStringVTable.setFont = _leFixedString_SetFont;
    fixedStringVTable.setFromString = _leFixedString_SetFromString;
	fixedStringVTable.setFromChar = _leFixedString_SetFromChar;
	fixedStringVTable.setFromCStr = _leFixedString_SetFromCStr;
    fixedStringVTable.charAt = _leFixedString_CharAt;
    fixedStringVTable.length = _leFixedString_Length;
    fixedStringVTable.isEmpty = _leFixedString_IsEmpty;
    fixedStringVTable.compare = _leFixedString_Compare;
    fixedStringVTable.append = _leFixedString_Append;
    fixedStringVTable.insert = _leFixedString_Insert;
    fixedStringVTable.remove = _leFixedString_Remove;
    fixedStringVTable.clear = _leFixedString_Clear;
    fixedStringVTable.toChar = _leFixedString_ToChar;
}