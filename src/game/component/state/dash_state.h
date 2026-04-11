#pragma once
#include "player_state.h"


namespace game::component::state {

    class DashState final : public PlayerState {
        friend class game::component::PlayerComponent;
    public:
        DashState(PlayerComponent* player_component) : PlayerState(player_component) {}
        ~DashState() override = default;

    private:
        void enter() override;
        void exit() override;
        std::unique_ptr<PlayerState> handleInput(engine::core::Context&) override;
        std::unique_ptr<PlayerState> update(float delta_time, engine::core::Context&) override;
    };
}
