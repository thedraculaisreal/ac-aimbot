#include "classes.h"

#define M_PI 3.14159265358979323846

float euclidean_distance(float x, float y) {
    return sqrtf((x * x) + (y * y));
}


void injected_thread()
{
    while (true)
    {

        // These variables will be used to hold the closest enemy to us
        float closest_player = -1.0f;
        float closest_yaw = NULL;
        float closest_pitch = NULL;

        HMODULE hModule = GetModuleHandle(nullptr);
        uintptr_t exe_base_address = (uintptr_t)GetModuleHandle(nullptr);
        Player* local_player = *(Player**)(exe_base_address + 0x17E0A8);
        int num_players = *(int*)(exe_base_address + 0x191FD4);

        if (!GetAsyncKeyState(VK_XBUTTON2))
            continue;

        for (int i = 1; i < num_players + 1; i++)
        {

            DWORD* entity_list = (DWORD*)(exe_base_address + 0x18AC04);
            DWORD* enemy_offset = (DWORD*)(*entity_list + (i * 4));
            Player* enemy = (Player*)(*enemy_offset);

            if (!local_player)
                continue;

            if (enemy->health > 100 || enemy->health <= 0)
                continue;

            float abspos_x = enemy->o.x - local_player->o.x;

            float abspos_y = enemy->o.y - local_player->o.y;

            float abspos_z = enemy->o.z - local_player->o.z;

            float temp_distance = euclidean_distance(abspos_x, abspos_y);
            if (closest_player == -1.0f || temp_distance < closest_player) // compares last loops enemy to new loop enemy seeing if closer 
                closest_player = temp_distance;

            // Calculate the yaw
            float azimuth_xy = atan2f(abspos_y, abspos_x);
            // Convert to degrees
            float yaw = (float)(azimuth_xy * (180.0 / M_PI));
            // Add 90 since the game assumes direct north is 90 degrees
            closest_yaw = yaw + 90;
            // don't look straight at the air when close to an enemy

            // Calculate the pitch
            // Since Z values are so limited, pick the larger between x and y to ensure that we 
            // don't look straight at the air when close to an enemy
            if (abspos_y < 0) {
                abspos_y *= -1;
            }
            if (abspos_y < 5) {
                if (abspos_x < 0) {
                    abspos_x *= -1;
                }
                abspos_y = abspos_x;
            }

            float azimuth_z = atan2f(abspos_z, abspos_y);
            // Covert the value to degrees
            closest_pitch = (float)(azimuth_z * (180.0 / M_PI));

        }

        if (closest_pitch == NULL || closest_yaw == NULL)
            continue;

        local_player->pitch = closest_pitch;
        local_player->yaw = closest_yaw;
       
        Sleep(1);
    }
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)injected_thread, NULL, NULL, NULL);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

