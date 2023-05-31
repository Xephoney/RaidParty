// Fill out your copyright notice in the Description page of Project Settings.


#include "TreasureHuntAIController.h"

ATreasureHuntAIController::ATreasureHuntAIController()
{
	bWantsPlayerState = true; 
}

void ATreasureHuntAIController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("AI good to go!"));
}

void ATreasureHuntAIController::RemoveTreasureFromArray(AActor* in)
{
	TreasuresInLevel.RemoveAt(TreasuresInLevel.Find(in));
	PrevTreasureDeleted = true;
}
