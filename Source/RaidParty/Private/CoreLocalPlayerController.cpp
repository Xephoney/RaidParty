// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreLocalPlayerController.h"

#include "BoardPawn.h"
#include "BoardPlayerState.h"
#include "BoardSpace.h"
#include "BoardTurnCharacter.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"

ACoreLocalPlayerController::ACoreLocalPlayerController()
{
	bAutoManageActiveCameraTarget = false;
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
		FinishedMoving(space->Type);
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
	if (MyRoll > 0)
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
	const auto input = Cast<UEnhancedInputComponent>(InputComponent);
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);
	if (ConfirmAction)
		input->BindAction(ConfirmAction, ETriggerEvent::Triggered, this, &ACoreLocalPlayerController::Confirm);
	if(PathSelectAction)
		input->BindAction(PathSelectAction, ETriggerEvent::Triggered, this, &ACoreLocalPlayerController::SelectPath);
	if (CameraToggleAction)
		input->BindAction(CameraToggleAction, ETriggerEvent::Triggered, this, &ACoreLocalPlayerController::CameraModeToggle);
	if (DirectionalAction)
		input->BindAction(DirectionalAction, ETriggerEvent::Triggered, this, &ACoreLocalPlayerController::JoystickInput);
	if (ConfirmReleasedAction)
		input->BindAction(ConfirmReleasedAction, ETriggerEvent::Triggered, this, &ACoreLocalPlayerController::ConfirmReleased);


}

void ACoreLocalPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(IsValid(State))
	{
		State->dt = DeltaSeconds;
		State->UpdateState();
	}

	if(bIsMyTurn)
	{
		FString DebugLog = "PlayerController Debug Info(" + FString::FromInt(PlayerIndex) + " : \n";
		DebugLog += "Select Paths | " + FString::FromInt(bSelectingPaths) + "\n";
		DebugLog += "Rolled       | " + FString::FromInt(bRolled) + "\n";
		DebugLog += "Camera Mode  | " + FString::FromInt(bCameraMode) + "\n";
		GEngine->AddOnScreenDebugMessage(675946584, 0.5f, FColor::Emerald, DebugLog);

		elapsed += DeltaSeconds;
		if(bRolling && elapsed > 0.05f)
		{
			MyRoll = FMath::RandRange(1, 10);
			UpdateRoll();
			elapsed = 0;
		}
	}
}

void ACoreLocalPlayerController::RollDice()
{
	bRolled = true;
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

void ACoreLocalPlayerController::BeginTurn(ABoardTurnCharacter* incharacter)
{
	TurnCharacter = incharacter;
	bIsMyTurn = true;
	Possess(incharacter);
	TurnCharacter->FollowTarget = myPawn;
	TurnCharacter->bFreeCameraMode = false;
}

void ACoreLocalPlayerController::Confirm(const FInputActionValue& Value)
{
	if (!bIsMyTurn)
		return;
	const bool Clicked = Value.Get<bool>();

	if (!bRolled )
	{
		bRolling = true;
		MyRoll = FMath::RandRange(1, 10);
		UpdateRoll();
		return;
	}

	if(Clicked)
	{
		TFunction<void()> Func;
		if(ConfirmQueue.Peek(Func))
			Func();
		ConfirmQueue.Pop();
	}
}

void ACoreLocalPlayerController::ConfirmReleased(const FInputActionValue& Value)
{
	
	if(bIsMyTurn && !bRolled && !bSelectingPaths && bRolling )
	{
		bRolling = false;
		bRolled = true;
		UpdateRoll();
		RollDice();
		bCameraMode = false;
		TurnCharacter->bFreeCameraMode = bCameraMode;
	}
}

void ACoreLocalPlayerController::CameraModeToggle(const FInputActionValue& Value)
{
	if(bIsMyTurn && Value.Get<bool>() == true)
	{
		bCameraMode = !bCameraMode;
		TurnCharacter->bFreeCameraMode = bCameraMode;
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

void ACoreLocalPlayerController::JoystickInput(const FInputActionValue& Value)
{
	if(bIsMyTurn && bCameraMode && IsValid(TurnCharacter) && Value.Get<FVector2D>().Size() > 0.f)
	{
		const FVector2D input = Value.Get<FVector2D>();
		const FString a = FString::SanitizeFloat(input.X) + " , " + FString::SanitizeFloat(input.Y);
		GEngine->AddOnScreenDebugMessage(413287, 0.5f, FColor::Purple, a);
		TurnCharacter->MoveCamera(input);
	}
}
