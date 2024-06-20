/*
 * Copyright (C) 2024 kyokuheishin <kyokuheishin@gmail.com>. All rights reserved.
 *
 * This file is part of libaribcaption.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <psp2/pvf.h>
#include "renderer/font_provider_psvita.hpp"

namespace aribcaption {

FontProviderPSVita::FontProviderPSVita(Context& context) : log_(GetContextLogger(context)) {}

FontProviderPSVita::~FontProviderPSVita() {
    if (pvfLibId != 0) {
        scePvfDoneLib(pvfLibId);
    }
}

FontProviderType FontProviderPSVita::GetType() {
    return FontProviderType::kPSVita;
}

bool FontProviderPSVita::Initialize() {
    ScePvfInitRec initParams = {
        NULL,
        4,
        NULL,
        NULL,
        PsVitaPvfAlloc,
        PsVitaPvfRealloc,
        PsVitaPvfFree
    };

    ScePvfError error;

    pvfLibId = scePvfNewLib(&initParams, &error);

    if (error != SCE_OK) {
        log_->e("PSVita: scePvfNewLib() failed: 0x%08X", error);
        return false;
    }

    return true;
}

void FontProviderPSVita::SetLanguage(uint32_t iso6392_language_code) {
    iso6392_language_code_ = iso6392_language_code;
}

auto FontProviderPSVita::GetFontFace(const std::string& font_name,
                                     std::optional<uint32_t> ucs4) -> Result<FontfaceInfo, FontProviderError> {


    ScePvfFamilyCode targetScePvfFamilyCode;
    ScePvfLanguageCode targetScePvfLanguageCode;
    ScePvfFontStyleInfo targetScePvfFontStyleInfo;
    ScePvfError error;

    if (font_name == "sans-serif") {
        targetScePvfFamilyCode = SCE_PVF_FAMILY_SANSERIF;
    } else if (font_name == "serif") {
        targetScePvfFamilyCode = SCE_PVF_FAMILY_SERIF;
    } else {
        targetScePvfFamilyCode = SCE_PVF_DEFAULT_FAMILY_CODE;
    }

    if (iso6392_language_code_ == ThreeCC("jpn")) {
        targetScePvfLanguageCode = SCE_PVF_LANGUAGE_J;
    } else {
        targetScePvfLanguageCode = SCE_PVF_LANGUAGE_LATIN;
    }

    memset(&targetScePvfFontStyleInfo, 0, sizeof(targetScePvfFontStyleInfo));

    targetScePvfFontStyleInfo.languageCode = targetScePvfLanguageCode;
    targetScePvfFontStyleInfo.familyCode = targetScePvfFamilyCode;
    targetScePvfFontStyleInfo.style = SCE_PVF_STYLE_REGULAR;

    ScePvfFontIndex fontIndex = scePvfFindOptimumFont(pvfLibId, &targetScePvfFontStyleInfo, &error);

    if (error != SCE_OK) {
        log_->e("PSVita: scePvfFindOptimumFont() failed: 0x%08X", error);
        return Err(FontProviderError::kOtherError);
    }

    error = scePvfGetFontInfoByIndexNumber(pvfLibId, &targetScePvfFontStyleInfo, fontIndex);

    if (error != SCE_OK) {
        log_->e("PSVita: scePvfGetFontInfoByIndexNumber() failed: 0x%08X", error);
        return Err(FontProviderError::kOtherError);
    }

    std::string familyName;

    if (targetScePvfFontStyleInfo.familyCode == SCE_PVF_FAMILY_SANSERIF) {
        familyName = "sans-serif";
    } else {
        familyName = "default";
    }

    FontfaceInfo info;
    info.family_name = targetScePvfFontStyleInfo.familyCode;
    info.postscript_name = "";
    std::string base_font_path = "sa0:data/font/pvf/";
    info.filename = base_font_path + reinterpret_cast<char*>(targetScePvfFontStyleInfo.fileName);
    info.face_index = -1;
    info.provider_type = FontProviderType::kPSVita;
    
    return Ok(std::move(info));    

}

ScePvfPointer FontProviderPSVita::PsVitaPvfAlloc(ScePvfPointer data, ScePvfU32 size){
    return (malloc(size));
}

ScePvfPointer FontProviderPSVita::PsVitaPvfRealloc(ScePvfPointer data, ScePvfPointer oldP, ScePvfU32 size){
    return (realloc(oldP, size));
}

void FontProviderPSVita::PsVitaPvfFree(ScePvfPointer data, ScePvfPointer p){
    free(p);
}



} // namespace aribcaption