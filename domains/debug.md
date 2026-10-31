# Project Debug Domain — CatCafe

## Log File

`C:\Users\jamie\Documents\Unreal Projects\CatCafe\Saved\Logs\CatCafe.log`

## Input Actions

For PIE injection:

| Action | Path | Type |
|--------|------|------|
| IA_Move | `/Game/Input/Actions/IA_Move` | Axis2D |
| IA_Look | `/Game/Input/Actions/IA_Look` | Axis2D |
| IA_MouseLook | `/Game/Input/Actions/IA_MouseLook` | Axis2D |
| IA_Jump | `/Game/Input/Actions/IA_Jump` | Digital; Pressed + Released triggers |

## Mapping Contexts

- `IMC_Default` — `/Game/Input/IMC_Default` (12 mappings: WASD/arrows, Space/gamepad jump, gamepad sticks).
- `IMC_MouseLook` — `/Game/Input/IMC_MouseLook` (Mouse2D → IA_MouseLook, Negate modifier).
- Player controller uses `IMC_Default` by default; `IMC_MouseLook` is in `MobileExcludedMappingContexts`.

## Current Map

`Lvl_FirstPerson` — 92 actors total, 82 gameplay actors.
Key actor types:
- `StaticMeshActor` — 41 (building shell, floor, counter, table, walls, road, blocks)
- `BP_StreetLight_C` — 8
- `BP_CeilingLamp_C` — 5
- `BP_Chair_C` — 3
- `BP_CoffeeCup_C` — 3

Also present: `BP_CatSpawner`, `BP_CoffeeMachine`, `BP_Coin`, `BP_FrontDoor`, `BP_Grinder`, `BP_KnockBox`, `BP_MilkCarton`, `BP_MilkJug` ×2, `BP_Portafilter` ×2, `BP_SteamWand`, `BP_Tamper`, `PlayerStart`, post-process volumes.

## Notes

- Coin flow: `BP_CatController` spawns `BP_Coin` on coffee receipt; `ACatCafeGameState::AddCoins` updates `CoinCount`; `WBP_HUD.CoinCount` updates through the GameState delegate.
- Useful PIE checks: serve a cat coffee, verify coin actor appears, HUD count increments, and no `BindWidget` warnings appear in the log.
