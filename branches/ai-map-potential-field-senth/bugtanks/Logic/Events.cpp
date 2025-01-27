/**
 * @file
 * @author Gustav Persson <ninjamuff@gmail.com>
 * @version 1.0
 * Copyright (�) A-Team.
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 * 
 * @section DESCRIPTION
 *
 * Events.cpp is where you put events!
 */
#include "Events.h"
#include "../Ai/AiEvents.h"

using namespace logic;

//System Events
const EventType EvtData_System_StartGame::msEventType("start_game");
const EventType EvtData_System_StartLoading::msEventType("start_loading");
const EventType EvtData_System_FinishedLoading::msEventType("Finished_Loading");
const EventType EvtData_System_EndGame::msEventType("end_game");
const EventType EvtData_System_PlayerDropped::msEventType("Player_Dropped");
const EventType EvtData_System_GameOver::msEventType("game_over");
const EventType EvtData_System_EventSystemStarted::msEventType("event_system_started");
const EventType EvtData_System_GFXSystemStarted::msEventType("GFX_system_started");
const EventType EvtData_System_ActorHandlerStarted::msEventType("actor_handler_Started");
const EventType EvtData_System_InputStatesStarted::msEventType("input_states_started");
const EventType EvtData_System_PreSave::msEventType("pre_save");
const EventType EvtData_System_PostSave::msEventType("post_save");
const EventType EvtData_System_MouseMove::msEventType("mouse_move");
const EventType EvtData_System_MouseKeyDown::msEventType("mouse_key_down");
const EventType EvtData_System_MouseKeyUp::msEventType("mouse_key_up");
const EventType EvtData_System_KeysPressed::msEventType("keys_pressed");

//From-Events
const EventType EvtData_From_ActorCreated::msEventType("actor_created");
const EventType EvtData_From_ActorDestroyed::msEventType("actor_destroyed");
const EventType EvtData_From_ActorCollisionRay::msEventType("actor_collision_ray");
const EventType EvtData_From_ActorCollisionObject::msEventType("actor_collsion_object");
const EventType EvtData_From_ActorHandlerSyncActors::msEventType("sync_actors");
const EventType EvtData_From_DriverDeath::msEventType("driver_death");
const EventType EvtData_From_DriverHit::msEventType("driver_hit");
const EventType EvtData_From_DriverEnterTank::msEventType("driver_enter_tank");
const EventType EvtData_From_EnemyHit::msEventType("enemy_hit");
const EventType EvtData_From_WeaponFired::msEventType("weapon_fired");
const EventType EvtData_From_WeaponSecondaryFired::msEventType("pedestrian_gun_fired");
const EventType EvtData_From_EnemyWeaponFired::msEventType("enemy_weapon_fired");
const EventType EvtData_From_KamikazeAntExplode::msEventType("kamikaze_ant_explode");
const EventType EvtData_From_EnemyMeleeAttack::msEventType("enemy_melee_attack");

//To-Events
const EventType EvtData_To_ActorCreateTank::msEventType("actor_create_tank");
const EventType EvtData_To_ActorCreateEnemy::msEventType("actor_create_enemy");
const EventType EvtData_To_ActorCreateMunition::msEventType("actor_create_munition");
const EventType EvtData_To_ActorCreateDriver::msEventType("actor_create_driver");
const EventType EvtData_To_ActorCreateSpectator::msEventType("actor_create_spectator");
const EventType EvtData_To_ActorCreateStaticEnvironment::msEventType("actor_create_StaticEnvironment");
const EventType EvtData_To_ActorStartMove::msEventType("actor_start_move");
const EventType EvtData_To_ActorStopMove::msEventType("actor_stop_move");
const EventType EvtData_To_EnemyUpdateHp::msEventType("enemy_update_hp");
const EventType EvtData_To_InputChangeGameState::msEventType("input_change_game_state");
const EventType EvtData_To_TankStartFire::msEventType("tank_start_fire");
const EventType EvtData_To_TankStopFire::msEventType("tank_stop_fire");
const EventType EvtData_To_DriverStartFire::msEventType("driver_start_fire");
const EventType EvtData_To_DriverStopFire::msEventType("driver_stop_fire");
const EventType EvtData_To_TankStartFireSecondary::msEventType("tank_start_fire_secondary");
const EventType EvtData_To_TankStopFireSecondary::msEventType("tank_stop_fire_secondary");
const EventType EvtData_To_ActorStartRotate::msEventType("actor_start_rotate");
const EventType EvtData_To_ActorStopRotate::msEventType("actor_stop_rotate");
const EventType EvtData_To_TankChangeWeapon::msEventType("tank_change_weapon");
const EventType EvtData_To_DriverStrafe::msEventType("driver_strafe");
const EventType EvtData_To_CameraStartRotate::msEventType("camera_start_rotate");
const EventType EvtData_To_CameraStopRotate::msEventType("camera_stop_rotate");
const EventType EvtData_To_ActorReduceHp::msEventType("actor_reduce_hp");
const EventType EvtData_To_CreateTrigger::msEventType("create_trigger");
const EventType EvtData_To_ShowTriggers::msEventType("show_triggers");
const EventType EvtData_To_HideTriggers::msEventType("hide_triggers");
const EventType EvtData_To_TriggerCollision::msEventType("trigger_collision");

// ---------------------------------
// AI-Events
// ---------------------------------
const EventType ai::EvtData_To_AiTogglePause::msEventType("ai_toggle_pause");
const EventType ai::EvtData_To_AgentAiGoals::msEventType("agent_ai_goals");
const EventType ai::EvtData_To_AiAddToConflictMap::msEventType("ai_add_to_conflict_map");
const EventType ai::EvtData_To_AiRemoveFromConflictMap::msEventType("ai_remove_from_conflict_map");