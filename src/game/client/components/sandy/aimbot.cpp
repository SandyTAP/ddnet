#include "aimbot.h"

#include <engine/shared/config.h>
#include <game/client/gameclient.h>
#include <game/client/prediction/entities/character.h>

// поиск ближайшей цели
int CAimbot::FindClosestTarget(vec2 From, float MaxDist)
{
    int BestId = -1;
    float BestDist = MaxDist;

    for(int i = 0; i < MAX_CLIENTS; i++)
    {
        if(i == GameClient()->m_Snap.m_LocalClientId)
            continue;

        if(!GameClient()->m_Snap.m_aCharacters[i].m_Active)
            continue;

        vec2 Pos = GameClient()->m_aClients[i].m_Predicted.m_Pos;
        float Dist = distance(From, Pos);

        if(Dist < BestDist)
        {
            BestDist = Dist;
            BestId = i;
        }
    }

    return BestId;
}

void CAimbot::OnUpdate()
{
    if(!g_Config.m_SandyAim)
        return;

    int LocalId = GameClient()->m_Snap.m_LocalClientId;
    if(LocalId < 0)
        return;

    CCharacter *pLocal = GameClient()->m_PredictedWorld.GetCharacterById(LocalId);
    if(!pLocal)
        return;

    vec2 LocalPos = pLocal->GetPos();

    // FOV (дистанция)
    float Fov = g_Config.m_SandyAimFov > 0 ? g_Config.m_SandyAimFov : 400.0f;

    int TargetId = FindClosestTarget(LocalPos, Fov);
    if(TargetId == -1)
        return;

    vec2 TargetPos = GameClient()->m_aClients[TargetId].m_Predicted.m_Pos;

    // направление
    vec2 Aim = TargetPos - LocalPos;

    // текущий прицел (ВАЖНО — берём vec2, а не собираем его)
    int Dummy = g_Config.m_ClDummy;
    vec2 Current = GameClient()->m_Controls.m_aMousePos[Dummy];

    // плавность
    float Smooth = 0.2f;

    vec2 NewAim = Current + (Aim - Current) * Smooth;

    // записываем обратно vec2
    GameClient()->m_Controls.m_aMousePos[Dummy] = NewAim;
}