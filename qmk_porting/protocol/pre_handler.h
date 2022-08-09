#pragma once

#ifdef NKRO_ENABLE
// a special trick
#define PROTOCOL_LUFA
#define ENDPOINT_TOTAL_ENDPOINTS 8
#else
#ifdef FORCE_NKRO
#undef FORCE_NKRO
#endif
#endif
#ifdef VIA_ENABLE
#ifndef DYNAMIC_KEYMAP_ENABLE
#define DYNAMIC_KEYMAP_ENABLE
#endif
#ifndef RAW_ENABLE
#define RAW_ENABLE
#endif
#endif
#ifdef DYNAMIC_KEYMAP_ENABLE
#ifndef EEPROM_ENABLE
#define EEPROM_ENABLE
#endif
#endif