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
	MyRoll--;
	State->BoardIndex = space->UniqueIndex;

	ArrivedAtSpace(space);

	if(MyRoll <= 0)
	{
		bIsMyTurn = false;
		myPawn->PawnFinishedMove();
		FinishedMoving(space->Type);
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

	myPawn->Move();
}

void ABoardAIController::ContinueMovement()
{
	if (MyRoll > 0)
		myPawn->Move();
	else
		EndTurn();

}

void ABoardAIController::ActivatePathSelect(const ABoardSpace& space)
{

}

void ABoardAIController::BeginPlay()
{
	Super::BeginPlay();
	State = GetPlayerState<ABoardPlayerState>();
	if (!State)
		GEngine->AddOnScreenDebugMessage(53253, 10.f, FColor::Red, FString("FAILED TO GET PLAYER STATE"));

}

void ABoardAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (IsValid(State))
	{
		State->dt = DeltaSeconds;
		State->UpdateState();
	}
	if (bIsMyTurn)
	{
		FString DebugLog;
		DebugLog += "| Select Paths  | " + FString::Printf(TEXT("%s\n"), bSelectingPaths ? TEXT("true") : TEXT("false"));
		DebugLog += "| Rolled        | " + FString::Printf(TEXT("%s\n"), bRolled ? TEXT("true") : TEXT("false"));
	
		GEngine->AddOnScreenDebugMessage(675946584, 0.5f, FColor::Purple, DebugLog);
	};
}

void ABoardAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}
