#include <vitasdk.h>
#include <vitaGL.h>
#include <imgui_vita.h>
#include <stdio.h>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "../loader/stb_image.h"

extern "C" {
void *__wrap_memcpy(void *dest, const void *src, size_t n) {
  return sceClibMemcpy(dest, src, n);
}

void *__wrap_memmove(void *dest, const void *src, size_t n) {
  return sceClibMemmove(dest, src, n);
}

void *__wrap_memset(void *s, int c, size_t n) {
  return sceClibMemset(s, c, n);
}
}

#define CONFIG_FILE_PATH "ux0:data/gtasa/config.txt"
#define CONTROLS_FILE_PATH "ux0:data/gtasa/controls.txt"

#define SKYGFX_COLOR_FILTER_NUM 4
enum SkyGfxColorFilter {
  SKYGFX_COLOR_FILTER_MOBILE,
  SKYGFX_COLOR_FILTER_NONE,
  SKYGFX_COLOR_FILTER_PS2,
  SKYGFX_COLOR_FILTER_PC,
};

char *SkyGfxColorFilterName[SKYGFX_COLOR_FILTER_NUM] = {
  "Mobile",
  "Откл.",
  "PS2",
  "PC"
};

#define RESOLUTION_NUM 3
const char *ResolutionName[RESOLUTION_NUM] = {
  "544p",
  "720p",
  "1080i"
};

#define ANTI_ALIASING_NUM 3
char *AntiAliasingName[ANTI_ALIASING_NUM] = {
  "Откл.",
  "MSAA 2x",
  "MSAA 4x"
};

#define CONTROLS_NUM 21
char *ControlsName[CONTROLS_NUM] = {
  "Не используется",
  "Cross",
  "Circle",
  "Square",
  "Triangle",
  "L1",
  "R1",
  "L3",
  "R3",
  "L2",
  "R2",
  "Select",
  "Start",
  "Вверх",
  "Вниз",
  "Влево",
  "Вправо",
  "Левый стик (Ось X)",
  "Левый стик(Ось Y)",
  "Правый стик (Ось X)",
  "Правый стик (Ось Y)"
};

char *ControlsVar[CONTROLS_NUM] = {
  "BUTTON_UNUSED",
  "BUTTON_CROSS",
  "BUTTON_CIRCLE",
  "BUTTON_SQUARE",
  "BUTTON_TRIANGLE",
  "BUTTON_L1",
  "BUTTON_R1",
  "BUTTON_L3",
  "BUTTON_R3",
  "BUTTON_L2",
  "BUTTON_R2",
  "BUTTON_SELECT",
  "BUTTON_START",
  "DPAD_UP",
  "DPAD_DOWN",
  "DPAD_LEFT",
  "DPAD_RIGHT",
  "ANALOG_LEFT_X",
  "ANALOG_LEFT_Y",
  "ANALOG_RIGHT_X",
  "ANALOG_RIGHT_Y"
};

#define CONTROLS_MAPPINGS_NUM 110
char *ControlsMapName[CONTROLS_MAPPINGS_NUM] = {
  "Атака",
  "Бег",
  "Прыжок",
  "Присесть",
  "Сесть в транспорт",
  "Тормоз",
  "Ручной тормоз",
  "Ускорение",
  "Изменить камеру: Приблизить",
  "Изменить камеру: Отдалить",
  "Гудок",
  "Предыдщуая радиостанция",
  "Следующая радиостанция",
  "Показать статистику",
  "Следующее оружие",
  "Предыдущее оружие",
  "Карта",
  "Посмотреть назад (Пешком)",
  "Посмотреть назад (Транспорт)",
  "Взгляд вправо (Транспорт)",
  "Взгляд влево (Транспорт)",
  "Начать/Отменить миссию",
  "Начать/Отменить побочное задание",
  "Движение (Ось X)",
  "Движение (Ось Y)",
  "Движение влево",
  "Движение вправо",
  "Обзор (Ось X)",
  "Обзор (Ось Y)",
  "Идти (Ось X)",
  "Идти (Ось Y)",
  "Гидравлика",
  "Сменить оружие в слоте",
  "Прицеливание: Приблизить",
  "Прицеливание: Отдалить",
  "Прицеливание",
  "Активировать бомбу (Транспорт)",
  "Башня: Влево",
  "Башня: Вправо",
  "Магнит",
  "Пропустить кат-сцену",
  "Банда: Нанять",
  "Банда: Игнорировать",
  "Банда: Следовать",
  "Банда: Удерживать позицию",
  "Ритм: Вверх",
  "Ритм: Вниз",
  "Ритм: Влево",
  "Ритм: Вправо",
  "Кран: Опустить",
  "Бросить предмет",
  "Телефон",
  "Нитро",
  "Кран: Вверх",
  "Кран: Вниз",
  "Принять",
  "Отменить",
  "Взять",
  "Стингер (вооружение)",
  "Меню: Вниз",
  "Меню: Вверх",
  "Меню: Влево",
  "Меню: Вправо",
  "Меню: Принять",
  "Меню: Назад",
  "Меню: Карта",
  "Игровой автомат: Кнопка",
  "Игровой автомат: Выход",
  "Игровой автомат: Сброс",
  "Игровой автомат: Джойстик",
  "Тренировка: Действие",
  "Тренировка: Уровень ниже",
  "Тренировка: Уровень выше",
  "Блэкджек: Сплит",
  "Блэкджек: Дабл",
  "Блэкджек: Хит",
  "Блэкджек: Стэнд",
  "Сделать ставку",
  "Отменить ставку",
  "Следующая цель",
  "Предыдущая цель",
  "Карта: Метка",
  "Вертолет: Магнит вверх",
  "Вертолет: Магнит вниз",
  "Гидравлика: Блокировка",
  "Полет: Подъем",
  "Полет: Снижение",
  "Полет: Основная атака",
  "Полет: Доп. атака",
  "Полет: Влево",
  "Полет: Вправо",
  "Полет: Вверх",
  "Полет: Вниз",
  "Баскетбол: Бросок",
  "Велосипед: Банни-хоп",
  "Карта: Увеличить",
  "Карта: Уменьшить",
  "Альтернативная атака",
  "Блок",
  "Укрытие: Влево",
  "Укрытие: Вправо",
  "Самолет: Шасси",
  "Поцелуй",
  "Танец: Вверх",
  "Танец: Вниз",
  "Танец: Влево",
  "Танец: Вправо",
  "Повтор (запись)",
  "Такси: Гидравлический прыжок",
  "Свободный обзор"
};

char *ControlsMapVar[CONTROLS_MAPPINGS_NUM] = {
  "MAPPING_ATTACK",
  "MAPPING_SPRINT",
  "MAPPING_JUMP",
  "MAPPING_CROUCH",
  "MAPPING_ENTER_CAR",
  "MAPPING_BRAKE",
  "MAPPING_HANDBRAKE",
  "MAPPING_ACCELERATE",
  "MAPPING_CAMERA_CLOSER",
  "MAPPING_CAMERA_FARTHER",
  "MAPPING_HORN",
  "MAPPING_RADIO_PREV_STATION",
  "MAPPING_RADIO_NEXT_STATION",
  "MAPPING_VITAL_STATS",
  "MAPPING_NEXT_WEAPON",
  "MAPPING_PREV_WEAPON",
  "MAPPING_RADAR",
  "MAPPING_PED_LOOK_BACK",
  "MAPPING_VEHICLE_LOOK_LEFT",
  "MAPPING_VEHICLE_LOOK_RIGHT",
  "MAPPING_VEHICLE_LOOK_BACK",
  "MAPPING_MISSION_START_AND_CANCEL",
  "MAPPING_MISSION_START_AND_CANCEL_VIGILANTE",
  "MAPPING_VEHICLE_STEER_X",
  "MAPPING_VEHICLE_STEER_Y",
  "MAPPING_VEHICLE_STEER_LEFT",
  "MAPPING_VEHICLE_STEER_RIGHT",
  "MAPPING_LOOK_X",
  "MAPPING_LOOK_Y",
  "MAPPING_PED_MOVE_X",
  "MAPPING_PED_MOVE_Y",
  "MAPPING_AUTO_HYDRAULICS",
  "MAPPING_SWAP_WEAPONS_AND_PURCHASE",
  "MAPPING_WEAPON_ZOOM_IN",
  "MAPPING_WEAPON_ZOOM_OUT",
  "MAPPING_ENTER_AND_EXIT_TARGETING",
  "MAPPING_VEHICLE_BOMB",
  "MAPPING_TURRET_LEFT",
  "MAPPING_TURRET_RIGHT",
  "MAPPING_MAGNET",
  "MAPPING_SKIP_CUTSCENE",
  "MAPPING_GANG_RECRUIT",
  "MAPPING_GANG_IGNORE",
  "MAPPING_GANG_FOLLOW",
  "MAPPING_GANG_HOLD_POSITION",
  "MAPPING_RHYTHM_UP",
  "MAPPING_RHYTHM_DOWN",
  "MAPPING_RHYTHM_LEFT",
  "MAPPING_RHYTHM_RIGHT",
  "MAPPING_DROP_CRANE",
  "MAPPING_DROP_ITEM",
  "MAPPING_PHONE",
  "MAPPING_NITRO",
  "MAPPING_CRANE_UP",
  "MAPPING_CRANE_DOWN",
  "MAPPING_ACCEPT",
  "MAPPING_CANCEL",
  "MAPPING_GRAB",
  "MAPPING_STINGER",
  "MAPPING_MENU_DOWN",
  "MAPPING_MENU_UP",
  "MAPPING_MENU_LEFT",
  "MAPPING_MENU_RIGHT",
  "MAPPING_MENU_ACCEPT",
  "MAPPING_MENU_BACK",
  "MAPPING_MENU_MAP",
  "MAPPING_ARCADE_BUTTON",
  "MAPPING_ARCADE_POWER_OFF",
  "MAPPING_ARCADE_RESET",
  "MAPPING_ARCADE_JOYSTICK",
  "MAPPING_GYM_ACTION",
  "MAPPING_GYM_EASIER_LEVEL",
  "MAPPING_GYM_HARDER_LEVEL",
  "MAPPING_BLACK_JACK_SPLIT",
  "MAPPING_BLACK_JACK_DOUBLE",
  "MAPPING_BLACK_JACK_HIT",
  "MAPPING_BLACK_JACK_STAND",
  "MAPPING_PLACE_BET",
  "MAPPING_REMOVE_BET",
  "MAPPING_NEXT_TARGET",
  "MAPPING_PREV_TARGET",
  "MAPPING_WAYPOINT_BLIP",
  "MAPPING_HELICOPTER_MAGNET_UP",
  "MAPPING_HELICOPTER_MAGNET_DOWN",
  "MAPPING_LOCK_HYDRAULICS",
  "MAPPING_FLIGHT_ASCEND",
  "MAPPING_FLIGHT_DESCEND",
  "MAPPING_FLIGHT_PRIMARY_ATTACK",
  "MAPPING_FLIGHT_SECONDARY_ATTACK",
  "MAPPING_FLIGHT_ALT_LEFT",
  "MAPPING_FLIGHT_ALT_RIGHT",
  "MAPPING_FLIGHT_ALT_UP",
  "MAPPING_FLIGHT_ALT_DOWN",
  "MAPPING_BASKETBALL_SHOOT",
  "MAPPING_BUNNY_HOP",
  "MAPPING_MAP_ZOOM_IN",
  "MAPPING_MAP_ZOOM_OUT",
  "MAPPING_ALT_ATTACK",
  "MAPPING_BLOCK",
  "MAPPING_TAKE_COVER_LEFT",
  "MAPPING_TAKE_COVER_RIGHT",
  "MAPPING_TOGGLE_LANDING_GEAR",
  "MAPPING_KISS",
  "MAPPING_DANCING_UP",
  "MAPPING_DANCING_DOWN",
  "MAPPING_DANCING_LEFT",
  "MAPPING_DANCING_RIGHT",
  "MAPPING_REPLAY",
  "MAPPING_TAXI_BOOST_JUMP",
  "MAPPING_ENTER_FREE_AIM"
};

int controls_map[CONTROLS_MAPPINGS_NUM];
int secondary_controls_map[CONTROLS_MAPPINGS_NUM];
int backup_controls_map[CONTROLS_MAPPINGS_NUM];
int backup_secondary_controls_map[CONTROLS_MAPPINGS_NUM];

int touch_x_margin = 100;
bool front_touch_triggers = false;
bool fix_heli_plane_camera = true;

int skygfx_colorfilter = SKYGFX_COLOR_FILTER_PS2;
bool skygfx_ps2_shading = true;
bool skygfx_ps2_sun = true;

int resolution = 0;
int aa_mode = SCE_GXM_MULTISAMPLE_2X;
bool high_detail_player = false;
bool detail_textures = false;
bool tex_bias = false;
bool mipmaps = true;
bool fix_skin_weights = true;
bool ped_spec = false;
bool mobile_stuff = false;
bool show_wanted_stars = true;
bool road_reflections = true;
bool car_fov_effects = true;

bool fuzzy_seek = false;
bool enable_mvp_optimization = false;
bool enable_bones_optimization = false;

bool show_controls_window = false;

void loadConfig(void) {
  char buffer[30];
  int value;

  FILE *config = fopen(CONFIG_FILE_PATH, "r");

  if (config) {
    while (EOF != fscanf(config, "%[^ ] %d\n", buffer, &value)) {
      if (strcmp("touch_x_margin", buffer) == 0) touch_x_margin = value;
      else if (strcmp("front_touch_triggers", buffer) == 0) front_touch_triggers = (bool)value;
      else if (strcmp("fix_heli_plane_camera", buffer) == 0) fix_heli_plane_camera = (bool)value;

      else if (strcmp("skygfx_colorfilter", buffer) == 0) skygfx_colorfilter = value;
      else if (strcmp("skygfx_ps2_shading", buffer) == 0) skygfx_ps2_shading = (bool)value;
      else if (strcmp("skygfx_ps2_sun", buffer) == 0) skygfx_ps2_sun = (bool)value;

      else if (strcmp("resolution", buffer) == 0) resolution = value;
      else if (strcmp("aa_mode", buffer) == 0) aa_mode = value;
      else if (strcmp("enable_high_detail_player", buffer) == 0) high_detail_player = (bool)value;
      else if (strcmp("disable_detail_textures", buffer) == 0) detail_textures = value ? false : true;
      else if (strcmp("disable_tex_bias", buffer) == 0) tex_bias = value ? false : true;
      else if (strcmp("disable_mipmaps", buffer) == 0) mipmaps = value ? false : true;
      else if (strcmp("fix_skin_weights", buffer) == 0) fix_skin_weights = (bool)value;
      else if (strcmp("disable_ped_spec", buffer) == 0) ped_spec = value ? false : true;
      else if (strcmp("ignore_mobile_stuff", buffer) == 0) mobile_stuff = value ? false : true;
      else if (strcmp("show_wanted_stars", buffer) == 0) show_wanted_stars = (bool)value;
      else if (strcmp("road_reflections", buffer) == 0) road_reflections = (bool)value;	
      else if (strcmp("car_fov_effects", buffer) == 0) car_fov_effects = (bool)value;	  

      else if (strcmp("enable_fuzzy_seek", buffer) == 0) fuzzy_seek = (bool)value;
      else if (strcmp("enable_mvp_optimization", buffer) == 0) enable_mvp_optimization = (bool)value;
      else if (strcmp("enable_bones_optimization", buffer) == 0) enable_bones_optimization = (bool)value;
    }
    fclose(config);
  }
}

void saveConfig(void) {
  FILE *config = fopen(CONFIG_FILE_PATH, "w+");

  if (config) {
    fprintf(config, "%s %d\n", "touch_x_margin", touch_x_margin);
    fprintf(config, "%s %d\n", "front_touch_triggers", (int)front_touch_triggers);
    fprintf(config, "%s %d\n", "fix_heli_plane_camera", (int)fix_heli_plane_camera);

    fprintf(config, "%s %d\n", "skygfx_colorfilter", skygfx_colorfilter);
    fprintf(config, "%s %d\n", "skygfx_ps2_shading", (int)skygfx_ps2_shading);
    fprintf(config, "%s %d\n", "skygfx_ps2_sun", (int)skygfx_ps2_sun);

    fprintf(config, "%s %d\n", "resolution", resolution);
    fprintf(config, "%s %d\n", "aa_mode", aa_mode);
    fprintf(config, "%s %d\n", "enable_high_detail_player", (int)high_detail_player);
    fprintf(config, "%s %d\n", "disable_detail_textures", detail_textures ? false : true);
    fprintf(config, "%s %d\n", "disable_tex_bias", tex_bias ? false : true);
    fprintf(config, "%s %d\n", "disable_mipmaps", mipmaps ? false : true);
    fprintf(config, "%s %d\n", "fix_skin_weights", (int)fix_skin_weights);
    fprintf(config, "%s %d\n", "disable_ped_spec", ped_spec ? false : true);
    fprintf(config, "%s %d\n", "ignore_mobile_stuff", mobile_stuff ? false : true);
    fprintf(config, "%s %d\n", "show_wanted_stars", (int)show_wanted_stars);
    fprintf(config, "%s %d\n", "road_reflections", (int)road_reflections);
    fprintf(config, "%s %d\n", "car_fov_effects", (int)car_fov_effects);

    fprintf(config, "%s %d\n", "enable_fuzzy_seek", (int)fuzzy_seek);
    fprintf(config, "%s %d\n", "enable_mvp_optimization", (int)enable_mvp_optimization);
    fprintf(config, "%s %d\n", "enable_bones_optimization", (int)enable_bones_optimization);
    fclose(config);
  }
}

bool areButtonsLoaded = false;
void loadButtons(void) {
  sceClibMemset(controls_map, 0, CONTROLS_MAPPINGS_NUM * sizeof(int));
  sceClibMemset(secondary_controls_map, 0, CONTROLS_MAPPINGS_NUM * sizeof(int));
  char str[256], map[64], val[64];
  int ctrl_idx = 0;
  if (!areButtonsLoaded) {
    FILE *config = fopen(CONTROLS_FILE_PATH, "r");
    while (fgets(str, 256, config)) {
      if (str[0] == ';' || str[0] == '\n') continue;
      sscanf(str, "%s %s", map, val);
      for (int i = 0; i < CONTROLS_MAPPINGS_NUM; i++) {
        if (!strcmp(map, ControlsMapVar[i])) {
          for (int j = 0; j < CONTROLS_NUM; j++) {
            if (!strcmp(val, ControlsVar[j])) {
              ctrl_idx = j;
              break;
            }
          }
          if (controls_map[i] && controls_map[i] != ctrl_idx)
              secondary_controls_map[i] = ctrl_idx;
          else
              controls_map[i] = ctrl_idx;
          break;
        }
      }
    }
    areButtonsLoaded = true;
    fclose(config);
  }
  
  sceClibMemcpy(backup_controls_map, controls_map, CONTROLS_MAPPINGS_NUM * sizeof(int));
  sceClibMemcpy(backup_secondary_controls_map, secondary_controls_map, CONTROLS_MAPPINGS_NUM * sizeof(int));
}

void saveButtons(void) {
  FILE *config = fopen(CONTROLS_FILE_PATH, "w+");
  
  fprintf(config, "%s\n", "; Vita-enhanced controls");
  fprintf(config, "\n");
  
  for (int i = 0; i < CONTROLS_MAPPINGS_NUM; i++) {
    fprintf(config, "%s %s\n", ControlsMapVar[i], ControlsVar[controls_map[i]]);
    if (secondary_controls_map[i] && secondary_controls_map[i] != controls_map[i])
      fprintf(config, "%s %s\n", ControlsMapVar[i], ControlsVar[secondary_controls_map[i]]);	
  }
  
  fclose(config);
}

char *options_descs[] = {
  "Мертвая зона для сенсорного экрана и задней панели. Задается в пикселях.\nПо умолчанию: 100.", // touch_x_margin
  "Перенос клавиш L2/R2 с задней панели на сенсорный экран.\nПо умолчанию: Отключено.", // front_touch_triggers
  "Позволяет использовать правый стик для обзора во время полета на самолетах и вертолетах.\nПо умолчанию: Включено.", // fix_heli_plane_camera

  "Выбор эффекта постобработки (светофильтр) для отображения в игре.\nПо умолчанию: PS2.", // skygfx_colorfilter
  "Включает эффекты затенения аналогично PS2-версии игры.\nПо умолчанию: Включено.", // skygfx_ps2_shading
  "Включает эффекты освещения, аналогично PS2-версии игры.\nПо умолчанию: Включено.", // skygfx_ps2_sun
  
  "Выбор разрешения в игре. Используйте плагин Sharpscale, если выбрано разрешение выше чем 544p.\nПо умолчанию: 544p.", // resolution
  "Выбор метода и уровня сглаживания в игре.\nПо умолчанию: MSAA 2x.", // aa_mode
  "Включает отображение более качественных текстур для модели игрока.\nПо умолчанию: Отключено.", // enable_high_detail_player
  "Включает отображение более качественных текстур.\nПо умолчанию: Отключено.", // disable_detail_textures
  "Включает точное смещение MIP-текстур. Увеличивает нагрузку на GPU.\nПо умолчанию: Отключено.", // disable_tex_bias
  "Использовать фильтрацию текстур с помощью MIP-карт. Увеличивает нагрузку на CPU, но уменьшает на GPU.\nПо умолчанию: Включено.", // disable_mipmaps
  "Аппаратная обработка моделей, позволяет исправить анимации, в том числе и лицевые.\nПо умолчанию: Включено.", // fix_skin_weights
  "Включает отражения на моделях NPC в игре.\nПо умолчанию: Отключено.", // disable_ped_spec
  "Включает отображение иконок и элементов из мобильной версии игры.\nПо умолчанию: Отключено.", // ignore_mobile_stuff
  "Постоянное отображение звезд розыска в HUD.\nПо умолчанию: Вкючено.",// show_wanted_stars
  "Включает отражения на дороге во время дождя.\nПо умолчанию: Включено.",// road_reflections
  "Отключает FOV-эффект при управлении транспортом. \nПо умолчанию: Отключено.",// car_fov_effects  

  "Ускоряет загрузку MP3-аудио, но обработка становится менее точной.\nПо умолчанию: Отключено.", // enable_fuzzy_seek
  "Перенос просчета MVP с GPU на CPU. Может улучшить производительность.\nПо умолчанию: Отключено.", // enable_mvp_optimization
  "Перенос просчета структуры моделей с CPU на GPU. Влияет на производительность.\nПо умолчанию: Отключено.", // enable_bones_optimization

  "Сохранить настройки и выйти из приложения.", // save_exit
  "Сохранить настройки и запустить игру.", // save_launch
  "Отменить последние изменения и выйти из приложения.", // cancel_exit
  "Отменить последние изменения и запустить игру.", // cancel_launch
};

enum {
  OPT_DEADZONE,
  OPT_FRONT_TOUCH_TRIGGERS,
  OPT_FLYING_VEHICLES_FIX,

  OPT_COLOR_FILTER,
  OPT_PS2_SHADING,
  OPT_PS2_SUN,

  OPT_RESOLUTION,
  OPT_ANTIALIASING,
  OPT_HI_DETAIL_PLAYER,
  OPT_DETAIL_TEX,
  OPT_TEX_BIAS,
  OPT_MIPMAPS,
  OPT_SKINNING_FIX,
  OPT_PED_SPEC,
  OPT_MOBILE_STUFF,
  OPT_WANTED_STARS,
  OPT_ROAD_REFLECTIONS,
  OPT_CAR_FOV_EFFECTS,  

  OPT_FUZZY_SEEK,
  OPT_MVP_OPT,
  OPT_BONES_OPT,
  
  SAVE_EXIT_OPT,
  SAVE_LAUNCH_OPT,
  CANCEL_EXIT_OPT,
  CANCEL_LAUNCH_OPT,
};

char *desc = nullptr;

void SetDescription(int i) {
  if (ImGui::IsItemHovered())
    desc = options_descs[i];
}

ImTextureID loadTex(const char* fname) {
    FILE* f = fopen(fname, "r");
    if (!f) return NULL;
    int image_width, image_height, depth;
    unsigned char* data = stbi_load_from_file(f, &image_width, &image_height, &depth, 0);
    fclose(f);

    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    return reinterpret_cast<ImTextureID>(image_texture);
}

ImVec2 FloorImVec2(const ImVec2& vec) {
    return ImVec2(std::floor(vec.x), std::floor(vec.y));
}

bool CustomTextButton(const char* label, ImVec4 normalColor, ImVec4 hoverColor, const char* unique_id) {
    std::string full_label = std::string(label) + "###" + unique_id;
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_NavHighlight, ImVec4(ImColor(0, 0, 0, 0)));
    bool pressed = ImGui::Button(full_label.c_str());
    ImGui::PopStyleColor(6);

    bool isFocused = ImGui::IsItemFocused();
    ImVec2 pos = ImGui::GetItemRectMin();
    ImVec2 size = ImGui::GetItemRectSize();
    ImVec2 window_pos = ImGui::GetWindowPos();
    ImVec2 raw_text_pos = ImVec2(
        window_pos.x + pos.x + 5.0f,
        window_pos.y + pos.y + (size.y - ImGui::GetFontSize()) * 0.5f
    );
	
    ImVec2 text_pos = FloorImVec2(raw_text_pos);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImColor text_color = isFocused ? ImColor(hoverColor) : ImColor(normalColor);
    draw_list->AddText(text_pos, text_color, label);
    
    return pressed;
}

int main(int argc, char *argv[]) {
  loadConfig();
  int exit_code = 0xDEAD;

  vglInitExtended(0, 960, 544, 0x1800000, SCE_GXM_MULTISAMPLE_4X);
  
  ImTextureID bg = loadTex("app0:/configuration_app.png");
  
  ImGui::CreateContext();
  ImGui_ImplVitaGL_Init();
  ImGui_ImplVitaGL_TouchUsage(false);
  ImGui_ImplVitaGL_GamepadUsage(true);
  ImGui::StyleColorsDark();
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

  ImGui::GetIO().MouseDrawCursor = false;
  
  ImGuiStyle& style = ImGui::GetStyle();
  style.FrameRounding = 2.0f;
  style.WindowBorderSize = 0.0f;
  
  ImFont* FontSm;
  ImFont* FontLg;
  ImFont* FontSa;

  ImGuiIO& io = ImGui::GetIO();
  FontSm = ImGui::GetIO().Fonts->AddFontFromFileTTF("centurygothic.ttf", 16, nullptr, io.Fonts->GetGlyphRangesCyrillic());
  FontLg = ImGui::GetIO().Fonts->AddFontFromFileTTF("centurygothic.ttf", 18, nullptr, io.Fonts->GetGlyphRangesCyrillic());
  FontSa = ImGui::GetIO().Fonts->AddFontFromFileTTF("pricedown.ttf", 24, nullptr, io.Fonts->GetGlyphRangesCyrillic());

  while (exit_code == 0xDEAD) {
    desc = nullptr;
    ImGui_ImplVitaGL_NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(960, 544), ImGuiSetCond_Always);
    ImGui::Begin("##main", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus);

    ImDrawList* idl = ImGui::GetWindowDrawList();
    idl->AddImage(bg, {0,0}, {960,544});

    ImGui::PushFont(FontLg);	
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
    ImGui::SetCursorPos({136, 60});	
    ImGui::Text("Мертвая зона сенсора:"); ImGui::SameLine();
    ImGui::PushItemWidth(494);
    ImGui::SliderInt("##touch_x_margin", &touch_x_margin, 0, 256);
    SetDescription(OPT_DEADZONE);

    ImGui::SetCursorPos({391, 91});	
    ImGui::Text("Перенести L2/R2:"); ImGui::SameLine();
    ImGui::Checkbox("##check14", &front_touch_triggers);	
    SetDescription(OPT_FRONT_TOUCH_TRIGGERS);

    ImGui::SetCursorPos({663, 91});	
    ImGui::Text("Обзор при полете:"); ImGui::SameLine();
    ImGui::Checkbox("##check1", &fix_heli_plane_camera);
    SetDescription(OPT_FLYING_VEHICLES_FIX);
    ImGui::PopStyleVar();	

    ImGui::SetCursorPos({136, 88});		
    if (ImGui::Button("Настройки управления")) {
      show_controls_window = !show_controls_window;
      if (show_controls_window) loadButtons();
    }

    ImGui::SetCursorPos({136, 153});	
    ImGui::Text("Фильтр:"); ImGui::SameLine();
    ImGui::PushItemWidth(89);
    if (ImGui::BeginCombo("##combo", SkyGfxColorFilterName[skygfx_colorfilter])) {
      for (int n = 0; n < SKYGFX_COLOR_FILTER_NUM; n++) {
        bool is_selected = skygfx_colorfilter == n;
        if (ImGui::Selectable(SkyGfxColorFilterName[n], is_selected))
          skygfx_colorfilter = n;
        SetDescription(OPT_COLOR_FILTER);
        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }
    SetDescription(OPT_COLOR_FILTER);
    ImGui::PopItemWidth();

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
    ImGui::SetCursorPos({391, 156});
    ImGui::Text("PS2-затенение:"); ImGui::SameLine();
    ImGui::Checkbox("##check2", &skygfx_ps2_shading);
    SetDescription(OPT_PS2_SHADING);

    ImGui::SetCursorPos({663, 156});	
    ImGui::Text("PS2-освещение:"); ImGui::SameLine();
    ImGui::Checkbox("##check3", &skygfx_ps2_sun);
    SetDescription(OPT_PS2_SUN);
    ImGui::PopStyleVar();

    ImGui::SetCursorPos({136, 220});	
    ImGui::Text("Разрешение:"); ImGui::SameLine();
    ImGui::PushItemWidth(100);
    if (ImGui::BeginCombo("##combor", ResolutionName[resolution])) {
      for (int n = 0; n < RESOLUTION_NUM; n++) {
        bool is_selected = resolution == n;
        if (ImGui::Selectable(ResolutionName[n], is_selected))
          resolution = n;
        SetDescription(OPT_RESOLUTION);
        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }
    SetDescription(OPT_RESOLUTION);	
    ImGui::PopItemWidth();
	
    ImGui::SetCursorPos({391, 220});	
    ImGui::Text("Сглаживание:"); ImGui::SameLine();
    ImGui::PushItemWidth(100);
    if (ImGui::BeginCombo("##combo2", AntiAliasingName[aa_mode])) {
      for (int n = 0; n < ANTI_ALIASING_NUM; n++) {
        bool is_selected = aa_mode == n;
        if (ImGui::Selectable(AntiAliasingName[n], is_selected))
          aa_mode = n;
        SetDescription(OPT_ANTIALIASING);
        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }
    SetDescription(OPT_ANTIALIASING);
    ImGui::PopItemWidth();

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));	
    ImGui::SetCursorPos({391, 254});	
    ImGui::Text("Детальные текстуры игрока:"); ImGui::SameLine();
    ImGui::Checkbox("##check15", &high_detail_player);
    SetDescription(OPT_HI_DETAIL_PLAYER);

    ImGui::SetCursorPos({136, 254});	
    ImGui::Text("Детальные текстуры:"); ImGui::SameLine();
    ImGui::Checkbox("##check4", &detail_textures);
    SetDescription(OPT_DETAIL_TEX);	

    ImGui::SetCursorPos({663, 254});	
    ImGui::Text("Смещение MIP:"); ImGui::SameLine();
    ImGui::Checkbox("##check5", &tex_bias);
    SetDescription(OPT_TEX_BIAS);

    ImGui::SetCursorPos({663, 223});	
    ImGui::Text("MIP-карты:"); ImGui::SameLine();
    ImGui::Checkbox("##check6", &mipmaps);
    SetDescription(OPT_MIPMAPS);

    ImGui::SetCursorPos({663, 282});	
    ImGui::Text("Исправление лиц:"); ImGui::SameLine();
    ImGui::Checkbox("##check7", &fix_skin_weights);
    SetDescription(OPT_SKINNING_FIX);

    ImGui::SetCursorPos({391, 282});	
    ImGui::Text("Отражения на NPC:"); ImGui::SameLine();
    ImGui::Checkbox("##check8", &ped_spec);
    SetDescription(OPT_PED_SPEC);	

    ImGui::SetCursorPos({663, 310});	
    ImGui::Text("Мобильные иконки:"); ImGui::SameLine();
    ImGui::Checkbox("##check11", &mobile_stuff);
    SetDescription(OPT_MOBILE_STUFF);

    ImGui::SetCursorPos({391, 310});
    ImGui::Text("Звезды розыска:"); ImGui::SameLine();	
    ImGui::Checkbox("##check17", &show_wanted_stars);
    SetDescription(OPT_WANTED_STARS);

    ImGui::SetCursorPos({136, 282});	
    ImGui::Text("Дорожные отражения:"); ImGui::SameLine();	
    ImGui::Checkbox("##check18", &road_reflections);
    SetDescription(OPT_ROAD_REFLECTIONS);

    ImGui::SetCursorPos({136, 310});	
    ImGui::Text("Откл. эффект FOV:"); ImGui::SameLine();	
    ImGui::Checkbox("##check19", &car_fov_effects);
    SetDescription(OPT_CAR_FOV_EFFECTS);

    ImGui::SetCursorPos({663, 374});	
    ImGui::Text("Упрощение MP3:"); ImGui::SameLine();
    ImGui::Checkbox("##check16", &fuzzy_seek);
    SetDescription(OPT_FUZZY_SEEK); ImGui::SameLine();	

    ImGui::SetCursorPos({136, 374});	
    ImGui::Text("Оптимизация MVP:"); ImGui::SameLine();
    ImGui::Checkbox("##check12", &enable_mvp_optimization);
    SetDescription(OPT_MVP_OPT); ImGui::SameLine();	

    ImGui::SetCursorPos({391, 374});	
    ImGui::Text("Оптимизация моделей:"); ImGui::SameLine();
    ImGui::Checkbox("##check13", &enable_bones_optimization);
    SetDescription(OPT_BONES_OPT);
    ImGui::PopStyleVar();	
    ImGui::PopFont();

    ImGui::PushFont(FontSa);
    ImVec4 normalColor = ImVec4(168.0f / 255.0f, 116.0f / 255.0f, 7.0f / 255.0f, 1.0f);
    ImVec4 hoverColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    ImGui::SetCursorPos(ImVec2(200, 406));
    if (CustomTextButton("выход", normalColor, hoverColor, "save_exit"))
        exit_code = 0;
    SetDescription(SAVE_EXIT_OPT);

    ImGui::SetCursorPos(ImVec2(285, 406));
    if (CustomTextButton("запустить игру", normalColor, hoverColor, "save_launch"))
        exit_code = 1;
    SetDescription(SAVE_LAUNCH_OPT);

    ImGui::SetCursorPos(ImVec2(463, 406));
    if (CustomTextButton("сброс и выход", normalColor, hoverColor, "cancel_exit"))
        exit_code = 2;
    SetDescription(CANCEL_EXIT_OPT);

    ImGui::SetCursorPos(ImVec2(620, 406));
    if (CustomTextButton("сброс и запуск", normalColor, hoverColor, "cancel_launch"))
        exit_code = 3;
    SetDescription(CANCEL_LAUNCH_OPT);

    ImGui::PopFont();

    if (desc) {
      ImGui::PushFont(FontSm);
      ImGui::SetCursorPos({202, 476});
      ImGui::PushTextWrapPos(780);	  
      ImGui::TextWrapped(desc);
      ImGui::PopFont();
    }
    ImGui::End();
  
    if (show_controls_window) {
      ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_Always);
      ImGui::SetNextWindowSize(ImVec2(960, 544), ImGuiSetCond_Always);
      ImGui::Begin("Настройки управления", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
      ImGui::SetCursorPosX(320);
      ImGui::PushItemWidth(150);
      if (ImGui::Button("Сохранить")) {
        show_controls_window = !show_controls_window;
        saveButtons();
      }
      ImGui::SameLine();
      if (ImGui::Button("Отмена")) {
        show_controls_window = !show_controls_window;
        sceClibMemcpy(controls_map, backup_controls_map, CONTROLS_MAPPINGS_NUM * sizeof(int));
        sceClibMemcpy(secondary_controls_map, backup_secondary_controls_map, CONTROLS_MAPPINGS_NUM * sizeof(int));
      }
      ImGui::PopItemWidth();
      ImGui::Columns(2, nullptr, false);
      ImGui::SetColumnWidth(0, 300);
      for (int i = 0; i < CONTROLS_MAPPINGS_NUM; i++) {
        std::string text = ControlsMapName[i];
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(text.c_str()).x - ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
        ImGui::Text(ControlsMapName[i]);
        ImGui::NextColumn();
        char cname[16];
        sprintf(cname, "##comboc%d", i);
        ImGui::PushItemWidth(ImGui::CalcTextSize("Analog Right (X Axis)").x + ImGui::GetStyle().FramePadding.x * 2.0f + 30);
        if (ImGui::BeginCombo(cname, ControlsName[controls_map[i]])) {
          for (int n = 0; n < CONTROLS_NUM; n++) {
            bool is_selected = controls_map[i] == n;
            if (ImGui::Selectable(ControlsName[n], is_selected))
              controls_map[i] = n;
            if (is_selected)
              ImGui::SetItemDefaultFocus();
          }
          ImGui::EndCombo();
        }
        ImGui::SameLine();
        sprintf(cname, "##combosc%d", i);
        if (ImGui::BeginCombo(cname, ControlsName[secondary_controls_map[i]])) {
          for (int n = 0; n < CONTROLS_NUM; n++) {
            bool is_selected = secondary_controls_map[i] == n;
            if (ImGui::Selectable(ControlsName[n], is_selected))
              secondary_controls_map[i] = n;
            if (is_selected)
              ImGui::SetItemDefaultFocus();
          }
          ImGui::EndCombo();
        }
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        
      }
      ImGui::Columns(1);
      ImGui::End();
    }
  
    glViewport(0, 0, static_cast<int>(ImGui::GetIO().DisplaySize.x), static_cast<int>(ImGui::GetIO().DisplaySize.y));
    ImGui::Render();
    ImGui_ImplVitaGL_RenderDrawData(ImGui::GetDrawData());
    vglSwapBuffers(GL_FALSE);
  }

  if (exit_code < 2) // Save
    saveConfig();

  if (exit_code % 2 == 1) // Launch
    sceAppMgrLoadExec("app0:/eboot.bin", NULL, NULL);

  return 0;
}
