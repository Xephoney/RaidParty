// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardPlayerState.h"
#include "BoardPawn.h"
#include "BoardTurnCharacter.h"

void ABoardPlayerState::UpdateState()
{
	if(oldCoins != Coins)
	{
		OnCoinsChanged.Broadcast();
		oldCoins = Coins;
	}
	if(oldKeeps != Keeps)
	{
		OnKeepsChanged.Broadcast();
		oldKeeps = Keeps;
	}
	if(oldTrickster != TricksterShrinePower)
	{
		OnTricksterChanged.Broadcast();
		oldTrickster = TricksterShrinePower;
	}
	if (oldTraveller != TravellerShrinePower)
	{
		OnTravellerChanged.Broadcast();
		oldTraveller = TravellerShrinePower;
	}
	if (oldTrader != TraderShrinePower)
	{
		OnTraderChanged.Broadcast();
		oldTrader = TraderShrinePower;
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
	TurnCharacter->FollowTarget = myPawn;
	TurnCharacter->bFreeCameraMode = false;
	BeginTurnDelegate.ExecuteIfBound();
}
