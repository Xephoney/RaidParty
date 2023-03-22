// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardPlayerState.h"

void ABoardPlayerState::UpdateState()
{
	if(oldCoins != Coins)
	{
		OnCoinsChanged.Broadcast();
		GEngine->AddOnScreenDebugMessage(55, 0.5f, FColor::Purple, FString("COINS CHANGED"));
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
}
