// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardPlayerState.h"
#include "BoardPawn.h"
#include "BoardTurnCharacter.h"

void ABoardPlayerState::UpdateState()
{
	if(oldCoins != Coins)
	{
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

	TurnStartEffects.Add([](ABoardPlayerState& state) -> int32
	{
		state.Coins += 2;
		return 2;
	});
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
