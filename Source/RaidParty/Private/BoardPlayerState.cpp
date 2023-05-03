// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardPlayerState.h"
#include "BoardPawn.h"
#include "BoardTurnCharacter.h"

void ABoardPlayerState::UpdateState()
{
	if(oldCoins != Coins)
	{
		for (auto func : CoinsChangedEffects)
		{
			func(*this);
		}
		Coins = FMath::Clamp(Coins, 0, 250);
		OnCoinsChanged.Broadcast(oldCoins, Coins - oldCoins);
		oldCoins = Coins;
	}
	if(oldKeeps != Keeps)
	{
		Keeps = FMath::Clamp(Keeps, 0, 250);
		OnKeepsChanged.Broadcast();
		oldKeeps = Keeps;
	}
	if(oldRanking != Ranking)
	{
		OnRankingChanged.Broadcast();
		oldRanking = Ranking;
	}
	if(oldColor != Color)
	{
		OnColorChanged.Broadcast();
		oldColor = Color;
	}

	if(MyRoll != OldRoll)
	{
		MyRoll = FMath::Clamp(MyRoll, 0, 250);
		OnMyRollChanged.Broadcast();
		OldRoll = MyRoll;
	}
}

void ABoardPlayerState::BeginPlay()
{
	Super::BeginPlay();
	//DEBUG EFFECT ADDED
	/*DiceRollEffects.Add([](ABoardPlayerState& state) -> int32
		{
			const int valuePreEffect = state.MyRoll;
			state.MyRoll = FMath::DivideAndRoundDown(state.MyRoll, 2);
			const int Mod = state.MyRoll - valuePreEffect;
			return Mod;
		});
	DiceRollEffects.Add([](ABoardPlayerState& state) -> int32
		{
			const int valuePreEffect = state.MyRoll;
			state.MyRoll = state.MyRoll + 2;
			const int Mod = state.MyRoll - valuePreEffect;
			return Mod;
		});*/

	/*TurnStartEffects.Add([](ABoardPlayerState& state) -> int32
	{
		state.Coins += 2;
		return 2;
	});*/
}

int32 ABoardPlayerState::RollDice()
{
	bRolled = true;
	bRolling = false;
	bPostRollPreMove = true;
	if(DiceRollEffects.Num() > 0)
	{
		TArray<int> mods;
		for (auto effect : DiceRollEffects)
		{
			mods.Add(effect(*this));
		}
		OnDiceRolledDelegate.Broadcast(OldRoll, mods);
		
		return mods.Num();
	}

	OnDiceRolledDelegate.Broadcast(OldRoll, {});

	
	return 0;
}

void ABoardPlayerState::SetCoins(int32 newCoinsBalance)
{
	Coins = newCoinsBalance;
	oldCoins = Coins;
}

void ABoardPlayerState::BeginTurn(ABoardTurnCharacter* inCharacter)
{
	TurnCharacter = inCharacter;
	TurnCharacter->FollowTarget = myPawn;
	TurnCharacter->bFreeCameraMode = false;
	bSelectingPaths = false;
	bCameraMode = false;
	bRollMode = false;
	bRolling = false;

	bPostRollPreMove = true;
	bIsMyTurn = true;
	if(TurnStartEffects.Num() > 0)
	{
		for (auto Func : TurnStartEffects)
		{
			Func(*this);
		}
	}
	
	BeginTurnDelegate.Broadcast(PlayerIndex);
} 

void ABoardPlayerState::EndTurn()
{
	EndTurnDelegate.Broadcast(PlayerIndex);
	TurnCharacter->bFreeCameraMode = false;
	bSelectingPaths = false;
	bCameraMode = false;
	bIsMyTurn = false;
	bRollMode = false;
	bRolling = false;

	bPostRollPreMove = true;
	
}

void ABoardPlayerState::AddEffect(int EffectIndex, int shrineIndex)
{
	TFunction<int(ABoardPlayerState&)> effect;
	switch (EffectIndex)
	{
		case 0 :
			effect = [](ABoardPlayerState& state) -> int32
			{
				const int valuePreEffect = state.MyRoll;
				state.MyRoll += 3;
				const int Mod = state.MyRoll - valuePreEffect;
				return Mod;
			};

			DiceRollEffects.Add(effect);
			ShrineEffectLinkArray.Add(std::make_tuple(shrineIndex, EffectIndex, DiceRollEffects.Num() - 1));
			return;

		case 1 :
			effect = [](ABoardPlayerState& state) -> int32
			{
				const int valuePreEffect = state.Coins;
				int Change = FMath::DivideAndRoundDown(state.MyRoll, 2);
				state.Coins += Change;
				FString output = "Recieving Half \nOld coins = " + FString::FromInt(state.oldCoins) + "\n";
				output += "Coins = " + FString::FromInt(state.Coins);
				output += "Change = " + FString::FromInt(Change);
				//GEngine->AddOnScreenDebugMessage(540983265, 10.f, FColor::Yellow, output);
				return 0;
			};
			DiceRollEffects.Add(effect);
			ShrineEffectLinkArray.Add(std::make_tuple(shrineIndex, EffectIndex, DiceRollEffects.Num() - 1));
			return ;

		case 2 :
			effect = [](ABoardPlayerState& state) -> int32
			{
				int Change = (state.Coins - state.oldCoins) * 2;
				state.Coins = state.oldCoins + Change;
				FString output = "Multiplying reward ||\nOld coins = " + FString::FromInt(state.oldCoins) + "\n";
				output += "Coins = " + FString::FromInt(state.Coins);
				output += "Change = " + FString::FromInt(Change);
				//GEngine->AddOnScreenDebugMessage(540983265, 5.f, FColor::Yellow, output);
				return Change;
			};
			CoinsChangedEffects.Add(effect);
			ShrineEffectLinkArray.Add(std::make_tuple(shrineIndex, EffectIndex, CoinsChangedEffects.Num() - 1));
			return ;
	}
}

void ABoardPlayerState::RemoveEffect(int EffectIndex, int shrineIndex)
{
	int removeIndex = -1;
	int removeEffectIndex = -1;
	int counter = 0;
	for(auto& [shrine, effect, index] : ShrineEffectLinkArray)
	{
		if(shrine == shrineIndex && effect == EffectIndex)
		{
			removeIndex = index;
			removeEffectIndex = effect;
			if(EffectIndex == 0)
			{
				DiceRollEffects.RemoveAt(index);
			}
			else if (EffectIndex == 1)
			{
				DiceRollEffects.RemoveAt(index);
			}
			else 
			{
				CoinsChangedEffects.RemoveAt(index);
			}
			ShrineEffectLinkArray.RemoveAt(counter);
			break;
		}
		counter++;
	}
	
	for (auto& [shrine, effect, index] : ShrineEffectLinkArray)
		if (effect == removeEffectIndex)
			if (index > removeIndex)
				index--;
}
