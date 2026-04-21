#pragma once
#include <game/client/component.h>

class CAimbot : public CComponent
{
public:
    int Sizeof() const override { return sizeof(*this); }

    void OnUpdate() override;

private:
    int FindClosestTarget(vec2 From, float Fov);
};