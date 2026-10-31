# Project Blueprints Domain — CatCafe

## Player AnimBPs

**ABP_Unarmed** (`/Game/Characters/Player/Anims/ABP_Unarmed`) — Skeleton: `SK_Mannequin`
- 24 graphs. State machines:
  - `Locomotion` — 2 states: Idle, Walk / Run.
  - `Main States` — 4 states: Locomotion, Jump, Fall Loop, Land.
- Key variables: `GroundSpeed`, `Direction`, `ShouldMove`, `IsFalling`, `Velocity`, `Character`, `MovementComponent`.
- EventGraph computes speed/direction and drives state transitions.

**ABP_FP_Copy** (`/Game/Characters/Player/ABP_FP_Copy`) — Skeleton: `SK_Mannequin`
- First-person mesh AnimBP; minimal AnimGraph using Copy Pose From Mesh from the body.

**ABP_Cat** (`/Game/Characters/Cat/ABP_Cat`) — Skeleton: `SKEL_Cat`
- `CatStateMachine` — 4 states: Walk, Jump, Sit, Drink.
- Key variables: `bIsJumping`, `bIsSitting`, `bIsDrinking`.

## Animation Blueprints

3 total: `ABP_Unarmed`, `ABP_FP_Copy`, `ABP_Cat`.
Player uses `ABP_Unarmed` for the third-person body mesh and `ABP_FP_Copy` for the first-person mesh.

## Blend Spaces

| Name | Type | Axes |
|------|------|------|
| BS_Idle_Walk_Run | 2D | Direction (-180..180), Speed (0..600) |

- 27 samples; idle/jog/walk animation set; SpringDamper interpolation.

## Key Blueprints

31 Blueprint-family assets: 20 regular/interface BPs, 4 widget BPs, 3 AnimBPs, 4 ControlRig BPs.

- **BP_FirstPersonCharacter** (`/Game/Characters/Player`) — parent: `ACatCafeCharacter`
  - 7 components: capsule, arrow, body mesh (`SKM_Manny_Simple`, `ABP_Unarmed`), first-person mesh (`ABP_FP_Copy`), camera, hold position, movement.
  - `HUDClass` = `WBP_HUD`.
- **BP_FirstPersonPlayerController** (`/Game/Characters/Player`) — parent: `ACatCafePlayerController`
  - `DefaultMappingContexts` = `IMC_Default`; `MobileExcludedMappingContexts` = `IMC_MouseLook`.
- **BP_FirstPersonGameMode** (`/Game/Level`) — parent: `ACatCafeGameMode`
  - DefaultPawn: `BP_FirstPersonCharacter`; PlayerController: `BP_FirstPersonPlayerController`.
  - GameState: `ACatCafeGameState`; HUD class remains engine `HUD` (HUD widget is created by the character).
- **BP_CatController** (`/Game/Characters/Cat`) — parent: `ACatController`
  - `CoinBlueprint` = `BP_Coin`; AI controller = `ACatAIController`; speech bubble and hold position components.
- **BP_CatSpawner** (`/Game/Characters/Cat`) — parent: `ACatSpawner`.
- **BP_CoffeeCup** (`/Game/Objects/Coffee/Cup`) — parent: `ACoffeeCup`.
- **BP_CoffeeMachine** (`/Game/Objects/Coffee/CoffeeMachine`) — parent: `ACoffeeMachine`.
- **BP_Coin** (`/Game/Objects/Coin`) — parent: `ACoin`; floating/fading coin effect.
- **WBP_HUD** (`/Game/UI`) — `Crosshair`, `InteractPromptText`, `InteractIcon`, `MoneyPouch`, `CoinCount`.
- **BP_SpeechBubble** (`/Game/Characters/Cat`) — WidgetBlueprint used by cat speech bubbles.
