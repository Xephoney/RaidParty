// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreLocalPlayerController.h"

#include "BoardPawn.h"
#include "BoardPlayerState.h"
#include "BoardSpace.h"
#include "BoardTurnCharacter.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "DrawDebugHelpers.h"

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
		ActivatePathSelect(*space);
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

	if(PathSelectHorizontalAction)
		input->BindAction(PathSelectHorizontalAction, ETriggerEvent::Triggered, this, &ACoreLocalPlayerController::SelectPathHorizontal);
	if (PathSelectVerticalAction)
		input->BindAction(PathSelectVerticalAction, ETriggerEvent::Triggered, this, &ACoreLocalPlayerController::SelectPathVertical);

	if (CameraToggleAction)
		input->BindAction(CameraToggleAction, ETriggerEvent::Triggered, this, &ACoreLocalPlayerController::CameraModeToggle);
	if (DirectionalAction)
		input->BindAction(DirectionalAction, ETriggerEvent::Triggered, this, &ACoreLocalPlayerController::JoystickInput);
	if (ConfirmReleasedAction)
		input->BindAction(ConfirmReleasedAction, ETriggerEvent::Triggered, this, &ACoreLocalPlayerController::ConfirmReleased);
	if (RollDiceAction)
	{
		input->BindAction(RollDiceAction, ETriggerEvent::Started, this, &ACoreLocalPlayerController::RollDiceBegin);
		input->BindAction(RollDiceAction, ETriggerEvent::Canceled, this, &ACoreLocalPlayerController::ConfirmReleased);
		input->BindAction(RollDiceAction, ETriggerEvent::Completed, this, &ACoreLocalPlayerController::ConfirmReleased);
	}
	if (CancelAction)
		input->BindAction(CancelAction, ETriggerEvent::Triggered, this, &ACoreLocalPlayerController::CancelActivated);


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
		FString DebugLog = "PlayerController Debug Info(" + FString::FromInt(PlayerIndex) + ") \n";
		DebugLog += "| Select Paths | " + FString::Printf(TEXT("%s\n"), bSelectingPaths ? TEXT("true") : TEXT("false"));
		DebugLog += "| Rolled       | " + FString::Printf(TEXT("%s\n"), bRolled ? TEXT("true") : TEXT("false"));
		DebugLog += "| Rolling      | " + FString::Printf(TEXT("%s\n"), bRolling ? TEXT("true") : TEXT("false"));
		DebugLog += "| Roll Mode    | " + FString::Printf(TEXT("%s\n"), bRollMode ? TEXT("true") : TEXT("false"));
		DebugLog += "| Camera Mode  | " + FString::Printf(TEXT("%s\n"), bCameraMode ? TEXT("true") : TEXT("false"));
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

void ACoreLocalPlayerController::RollDiceBegin()
{
	if (bRolled || !bIsMyTurn || !bRollMode)
		return;

	bRolling = true;
	bRollMode = true;
	MyRoll = FMath::RandRange(1, 10);
	UpdateRoll();
}

void ACoreLocalPlayerController::RollDice()
{
	bRolled = true;
	bRolling = false;
	UpdateRoll();
	if(myPawn->BoardSpace->HasMultiplePaths(1))
	{
		ActivatePathSelect(*myPawn->BoardSpace);
		return;
	}
	myPawn->Move();
}

void ACoreLocalPlayerController::CancelActivated()
{
	if (bRollMode)
		bRollMode = false;
	if (bCameraMode)
	{
		bCameraMode = false;
		TurnCharacter->bFreeCameraMode = false;
	}

}

void ACoreLocalPlayerController::ActivatePathSelect(const ABoardSpace& space)
{
	bSelectingPaths = true;
	myPawn->DisplayPaths(MyRoll);
	CurrentPathIndex = 0;
	MaxPathIndex = space.NextTiles.Num();

	PathDirections.Empty();
	for(const auto _space : space.NextTiles)
	{
		FVector dir = _space->GetActorLocation() - space.GetActorLocation();
		dir.Z = 0;
		dir.Normalize();
		PathDirections.Add(FVector2D{dir.X, dir.Y});	
	}

	// Logic when A is Pressed to accept path
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

	if(!bRolled && Clicked && !bRollMode)
	{
		bRollMode = true; 
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
	if(bIsMyTurn && !bRolled && bRolling )
	{
		bRolling = false;
		bRollMode = false;
		bCameraMode = false;
		TurnCharacter->bFreeCameraMode = bCameraMode;
		bRolled = true;
		UpdateRoll();
		RollDice();
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

void ACoreLocalPlayerController::SelectPathHorizontal(const FInputActionValue& Value)
{
	if(!bIsMyTurn || !bSelectingPaths)
		return;

	if (!bIsMyTurn)
		return;
	if (bSelectingPaths)
	{
		SelectPathFromDirection(FVector2D(0, Value.Get<float>()));
		return;
	}

}

void ACoreLocalPlayerController::SelectPathVertical(const FInputActionValue& Value)
{
	if (!bIsMyTurn)
		return;
	if(bSelectingPaths)
		SelectPathFromDirection(FVector2D(Value.Get<float>(), 0));

}

void ACoreLocalPlayerController::JoystickInput(const FInputActionValue& Value)
{
	if (!bIsMyTurn || !IsValid(TurnCharacter))
		return;

	const FVector2D input = Value.Get<FVector2D>();
	
	if(bCameraMode && input.Size() > 0.f)
		TurnCharacter->MoveCamera(input);
	
	else if(bSelectingPaths && input.Size() > 0.2f)
	{
		FVector2D normalizedInput = FVector2D(input.Y, input.X);
		normalizedInput = normalizedInput.GetSafeNormal();
		SelectPathFromDirection(normalizedInput);
	}
}

void ACoreLocalPlayerController::SelectPathFromDirection(FVector2D Direction)
{
	Direction.Normalize();
	Direction = Direction.GetRotated(TurnCharacter->GetActorRotation().Euler().Z);
	TArray<float> dotProducts;
	int BestIndex = 0;
	float BestDot = -1.f;
	for (int i = 0; i < PathDirections.Num(); i++)
	{
		const float dot = FVector2D::DotProduct(Direction, PathDirections[i]);
		if (dot > BestDot)
		{
			BestDot = dot;
			BestIndex = i;
		}
		dotProducts.Add(dot);
	}
	if (CurrentPathIndex != BestIndex)
	{
		CurrentPathIndex = BestIndex;
		myPawn->UpdatePaths(BestIndex);
	}
}
