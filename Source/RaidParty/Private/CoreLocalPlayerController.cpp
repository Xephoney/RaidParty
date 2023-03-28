// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreLocalPlayerController.h"

#include "BoardPawn.h"
#include "BoardPlayerState.h"
#include "BoardSpace.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"

ACoreLocalPlayerController::ACoreLocalPlayerController()
{
	bAutoManageActiveCameraTarget = false;
	//bFindCameraComponentWhenViewTarget = false;
}

void ACoreLocalPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetShowMouseCursor(true);
	State = GetPlayerState<ABoardPlayerState>();
	if(!PlayerState)
		GEngine->AddOnScreenDebugMessage(53253, 10.f, FColor::Red, FString("FAILED TO GET PLAYER STATE"));
	
}

//Gets called from BoardPawn when it arrives at the new space
// It determines the next action to take, based on the Space-type
void ACoreLocalPlayerController::PawnArrived(ABoardSpace* space)
{
	if(!space)
	{
		GEngine->AddOnScreenDebugMessage(0132042105, 10.f, FColor::Red, FString("INVALID SPACE!!"));
	}
	MyRoll--;
	State->BoardIndex = space->UniqueIndex;
	//GEngine->AddOnScreenDebugMessage(12455123, 2.f, FColor::Cyan, FString::FromInt(MyRoll));
	ArrivedAtSpace(space);
	// My turn has ended, but we still need to process the last space
	if(MyRoll == 0)
	{
		bIsMyTurn = false;
		myPawn->PawnFinishedMove();
		EndTurnOnSpace(space);
		FinishedMoving();
		return; 
	}

	if (space->HasMultiplePaths(1))
	{
		bSelectingPaths = true;
		myPawn->DisplayPaths(MyRoll);
		MaxPathIndex = space->NextTiles.Num();
		const TFunction<void()> SelectedPathLogic = [this]()
		{
			bSelectingPaths = false;
			myPawn->HidePaths();
			myPawn->Move(CurrentPathIndex);
			CurrentPathIndex = 0;
		};
		ConfirmQueue.Enqueue(SelectedPathLogic);
		return;
	}
	
	if(space->bHaltPlayerOnPass)
	{
		PlayerHalted(space->Type);
		return;
	}
	myPawn->Move();

}

void ACoreLocalPlayerController::ContinueMovement()
{
	myPawn->Move();
}

void ACoreLocalPlayerController::OnPossess(APawn* aPawn)
{
	bAutoManageActiveCameraTarget = false;
	Super::OnPossess(aPawn);
}

void ACoreLocalPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	auto input = Cast<UEnhancedInputComponent>(InputComponent);
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);
	if (ConfirmAction)
		input->BindAction(ConfirmAction, ETriggerEvent::Triggered, this, &ACoreLocalPlayerController::Confirm);
	if(PathSelectAction)
		input->BindAction(PathSelectAction, ETriggerEvent::Triggered, this, &ACoreLocalPlayerController::SelectPath);
}

void ACoreLocalPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(IsValid(State))
	{
		State->dt = DeltaSeconds;
		State->UpdateState();

	}
}

void ACoreLocalPlayerController::EndTurnOnSpace(ABoardSpace* space)
{

}

void ACoreLocalPlayerController::RollDice()
{
	bRolled = true;
	const FString output = "Roll Complete (" + FString::FromInt(MyRoll) + ")";
	GEngine->AddOnScreenDebugMessage(589566, 2.f, FColor::Green, output);

	if(myPawn->BoardSpace->HasMultiplePaths(1))
	{
		bSelectingPaths = true;
		myPawn->DisplayPaths(MyRoll);
		MaxPathIndex = myPawn->BoardSpace->NextTiles.Num();
		const TFunction<void()> SelectedPathLogic = [this]()
		{
			bSelectingPaths = false;
			myPawn->HidePaths();
			myPawn->Move(CurrentPathIndex);
			CurrentPathIndex = 0;
		};
		ConfirmQueue.Enqueue(SelectedPathLogic);
		return;
	}
	myPawn->Move();
}

void ACoreLocalPlayerController::Confirm(const FInputActionValue& Value)
{
	const bool Clicked = Value.Get<bool>();
	if (!bRolled || !bIsMyTurn)
		return;
	if(Clicked)
	{
		TFunction<void()> Func;
		if(ConfirmQueue.Peek(Func))
			Func();
		ConfirmQueue.Pop();
	}
}

void ACoreLocalPlayerController::SelectPath(const FInputActionValue& Value)
{
	if(!bIsMyTurn || !bSelectingPaths)
		return;

	//Left
	if(Value.Get<float>() > 0.01f && CurrentPathIndex != MaxPathIndex-1)
	{
		CurrentPathIndex++;
		myPawn->UpdatePaths(CurrentPathIndex);
	}
	//Right
	else if(Value.Get<float>() < -0.01f && CurrentPathIndex != 0)
	{
		CurrentPathIndex--;
		myPawn->UpdatePaths(CurrentPathIndex);
	}
	
}