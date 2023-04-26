// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardAIController.h"

#include "BoardPawn.h"
#include "BoardPlayerState.h"
#include "BoardSpace.h"

ABoardAIController::ABoardAIController()
{
	bWantsPlayerState = true;
}

void ABoardAIController::PawnArrived(ABoardSpace* space)
{
	if (!space)
	{
		GEngine->AddOnScreenDebugMessage(0132042105, 10.f, FColor::Red, FString("INVALID SPACE!!"));
		return;
	}
	State->MyRoll--;
	State->BoardIndex = space->UniqueIndex;

	ArrivedAtSpace(space);

	if(State->MyRoll <= 0)
	{
		
		State->myPawn->PawnFinishedMove();
		FinishedMoving(space->Type);
		State->bIsMyTurn = false;
		return;
	}

	if (space->HasMultiplePaths(1))
	{
		ActivatePathSelect(*space);
		return;
	}

	if (space->bHaltPlayerOnPass)
	{
		PlayerHalted(space->Type);
		return;
	}

	State->myPawn->Move();
}

void ABoardAIController::ContinueMovement()
{
	if (State->MyRoll > 0)
		State->myPawn->Move();
	else
		State->EndTurn();

}

void ABoardAIController::RollComplete()
{
	State->bRolled = true;
	State->bRolling = false;
	if (!State->myPawn->OnPawnArrivedAtNewSpace.IsBoundToObject(this))
		State->myPawn->OnPawnArrivedAtNewSpace.BindUObject(this, &ABoardAIController::PawnArrived);
	UpdateRoll();
	if(State->myPawn->BoardSpace->HasMultiplePaths(1))
	{
		ActivatePathSelect(*State->myPawn->BoardSpace);
		return;
	}
	State->myPawn->Move();
}

void ABoardAIController::ActivatePathSelect(const ABoardSpace& space)
{
	State->bSelectingPaths = true;
	State->bSelectingShrine = false;
	State->myPawn->DisplayPaths(State->MyRoll);
	MaxPathIndex = space.NextTiles.Num() - 1;
	CurrentPathIndex = FMath::RandRange(0, MaxPathIndex - 1);
}

void ABoardAIController::SelectNewPath()
{
	if (!State->bSelectingPaths)
		return;
	CurrentPathIndex = FMath::RandRange(0, MaxPathIndex);
	State->myPawn->UpdatePaths(CurrentPathIndex);
}

void ABoardAIController::PathSelected()
{
	if (!State->bSelectingPaths)
		return;

	State->bSelectingPaths = false;
	State->myPawn->HidePaths();
	State->myPawn->Move(CurrentPathIndex);
	CurrentPathIndex = 0;
}

void ABoardAIController::BeginPlay()
{
	Super::BeginPlay();
	State = GetPlayerState<ABoardPlayerState>();
	if (!State)
		GEngine->AddOnScreenDebugMessage(53253, 10.f, FColor::Red, FString("FAILED TO GET PLAYER STATE"));

	State->BeginTurnDelegate.BindUObject(this, &ABoardAIController::BeginTurn);
}

void ABoardAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (IsValid(State))
	{
		State->dt = DeltaSeconds;
		State->UpdateState();
	}
	if (State->bIsMyTurn)
	{
		FString DebugLog;
		DebugLog += "| Is it my turn?   | " + FString::Printf(TEXT("%s\n"), State->bIsMyTurn ? TEXT("true") : TEXT("false"));
		DebugLog += "| Selecting Paths  | " + FString::Printf(TEXT("%s\n"), State->bSelectingPaths ? TEXT("true") : TEXT("false"));
		DebugLog += "| Has Rolled?      | " + FString::Printf(TEXT("%s\n"), State->bRolled ? TEXT("true") : TEXT("false"));
		DebugLog += "| MyRoll           | " + FString::FromInt(State->MyRoll) + "\n";
		GEngine->AddOnScreenDebugMessage(675946584, 0.5f, FColor::Emerald, DebugLog);

		elapsed += DeltaSeconds;
		if (State->bRolling && elapsed > 0.05f)
		{
			State->MyRoll = FMath::RandRange(1, 10);
			UpdateRoll();
			timeSpentRolling += elapsed;
			elapsed = 0;
		}
	};
}

void ABoardAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}
