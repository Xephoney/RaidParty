// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardPlayerState.h"
#include "BoardPawn.h"
#include "BoardTurnCharacter.h"

void ABoardPlayerState::UpdateState()
{
	if(oldCoins != Coins)
	{
		OnCoinsChanged.Broadcast(Coins - oldCoins);
		oldCoins = Coins;
	}
	if(oldKeeps != Keeps)
	{
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
		OnMyRollChanged.Broadcast();
		OldRoll = MyRoll;
	}
}

void ABoardPlayerState::BeginTurn(ABoardTurnCharacter* inCharacter)
{
	TurnCharacter = inCharacter;
	bIsMyTurn = true;
	bRollMode = false;
	bRolling = false;
	bSelectingPaths = false;
	bSelectingShrine = false;
	bCameraMode = false;
	TurnCharacter->FollowTarget = myPawn;
	TurnCharacter->bFreeCameraMode = false;
	BeginTurnDelegate.ExecuteIfBound();
}

void ABoardPlayerState::EndTurn()
{
	
	bIsMyTurn = false;
	bRollMode = false;
	bRolling = false;
	bSelectingPaths = false;
	bSelectingShrine = false;
	bCameraMode = false;
	TurnCharacter->bFreeCameraMode = false;
	EndTurnDelegate.Broadcast();
}
