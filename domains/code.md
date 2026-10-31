# Project Code Domain — CatCafe

## Class Hierarchy

AActor
  ├─ ACharacter
  │    ├─ ACatCafeCharacter      (Source/CatCafe/Player/CatCafeCharacter.h)
  │    └─ ACatController         (Source/CatCafe/Cat/CatController.h) — also IInteractable
  ├─ AGameModeBase
  │    └─ ACatCafeGameMode       (Source/CatCafe/CatCafeGameMode.h)
  ├─ AGameStateBase
  │    └─ ACatCafeGameState      (Source/CatCafe/Subsystems/CatCafeGameState.h)
  ├─ APlayerController
  │    └─ ACatCafePlayerController
  ├─ AAIController
  │    └─ ACatAIController
  ├─ AFillable
  │    ├─ ACoffeeCup
  │    ├─ AGrinder
  │    ├─ AMilkJug
  │    └─ APortafilter
  └─ AActor
       ├─ AChair
       ├─ ACoin
       ├─ ACoffeeMachine / ACoffeeMachineButton
       ├─ AMilkCarton / ATamper / AKnockBox / ASteamWand / AGroupHead
       ├─ AFrontDoor / ACatSpawner
       └─ IInteractable implementations across coffee and environment objects

UUserWidget        → UCatCafeHUD
UAnimInstance      → UCatAnimInstance
UGameInstanceSubsystem → USeatingManager, UStencilManager

Key classes:
- `ACatCafeCharacter` — first-person character; owns HUD, held object, interaction trace, first-person mesh/camera.
- `ACatController` — cat AI character; seating, coffee-receiving, animation state, coin spawn and GameState credit.
- `ACatCafeGameState` — shared economy state; `CoinCount`, `AddCoins()`, `OnCoinCountChanged`.
- `ACatCafeGameMode` — sets `GameStateClass` to `ACatCafeGameState`.
- `ACatCafeHUD` — binds HUD widgets; updates `CoinCount` from the GameState delegate.
- `UCatAnimInstance` — cat animation flags: `bIsJumping`, `bIsSitting`, `bIsDrinking`.
- `USeatingManager` — GameInstance subsystem; chair allocation and release.

## Module

**CatCafe** (`Source/CatCafe/CatCafe.Build.cs`)
- Export macro: `CATCAFE_API`
- Dependencies: Core, CoreUObject, Engine, InputCore, EnhancedInput, AIModule, StateTreeModule, GameplayStateTreeModule, UMG, Slate, Niagara, NavigationSystem
- Source dirs: Cat/, Environment/, Interfaces/, Objects/, Player/, Subsystems/, UI/, Utils/

## Key Headers

- `Source/CatCafe/Player/CatCafeCharacter.h` — input, HUD creation, held-object logic.
- `Source/CatCafe/Cat/CatController.h` — cat AI, seating/coffee flow, coin blueprint.
- `Source/CatCafe/Subsystems/CatCafeGameState.h` — coin economy and change delegate.
- `Source/CatCafe/Cat/CatAnimInstance.h` — cat animation boolean parameters.
