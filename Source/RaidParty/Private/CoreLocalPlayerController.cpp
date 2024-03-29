// Laget av Hans Ola Hoftun


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
	State = GetPlayerState<ABoardPlayerState>();
	if(!State)
		GEngine->AddOnScreenDebugMessage(53253, 10.f, FColor::Red, FString("FAILED TO GET PLAYER STATE"));

	State->bAI = false;
	State->BeginTurnDelegate.AddUniqueDynamic(this, &ACoreLocalPlayerController::BeginTurn);
}

//Gets called from BoardPawn when it arrives at the new space
// It determines the next action to take, based on the Space-type
void ACoreLocalPlayerController::PawnArrived(ABoardSpace* space)
{
	if(!space)
	{
		GEngine->AddOnScreenDebugMessage(0132042105, 10.f, FColor::Red, FString("INVALID SPACE!!"));
	}
	State->MyRoll--;
	State->BoardIndex = space->UniqueIndex;
	
	ArrivedAtSpace(space);
	
	if(State->MyRoll <= 0)
	{
		State->bIsMyTurn = false;
		State->myPawn->PawnFinishedMove();
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
	State->myPawn->Move();

}

void ACoreLocalPlayerController::ContinueMovement()
{
	if (State->MyRoll > 0)
	{
		State->myPawn->Move();
		if (!State->myPawn->OnPawnArrivedAtNewSpace.IsBoundToObject(this))
			State->myPawn->OnPawnArrivedAtNewSpace.BindUObject(this, &ACoreLocalPlayerController::PawnArrived);

		if(State->bCameraMode)
		{
			State->bCameraMode = false;
			State->TurnCharacter->bFreeCameraMode = false;
			State->TurnCharacter->DefaultMode();
		}
	}
	else
		State->EndTurn();
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


	if(State->bIsMyTurn)
	{
		/*
		FString DebugLog = "PlayerController Debug Info(" + FString::FromInt(State->PlayerIndex) + ") \n";
		DebugLog += "| Select Paths  | " + FString::Printf(TEXT("%s\n"), State->bSelectingPaths ? TEXT("true") : TEXT("false"));
		DebugLog += "| Rolled        | " + FString::Printf(TEXT("%s\n"), State->bRolled ? TEXT("true") : TEXT("false"));
		DebugLog += "| Rolling       | " + FString::Printf(TEXT("%s\n"), State->bRolling ? TEXT("true") : TEXT("false"));
		DebugLog += "| Roll Mode     | " + FString::Printf(TEXT("%s\n"), State->bRollMode ? TEXT("true") : TEXT("false"));
		DebugLog += "| Camera Mode   | " + FString::Printf(TEXT("%s\n"), State->bCameraMode ? TEXT("true") : TEXT("false"));
		DebugLog += "| Confirm Stack | [" + FString::FromInt(ConfirmStack.Num()) + "]\n";
		DebugLog += "| Decline Stack | [" + FString::FromInt(DeclineStack.Num()) + "]\n";
		if (State->bSelectingPaths)
			DebugLog += "| Current Index/Max Index " + FString::FromInt(CurrentPathIndex) + " / " + FString::FromInt(MaxPathIndex);

		GEngine->AddOnScreenDebugMessage(675946584, 0.5f, FColor::Purple, DebugLog);
		*/
		if (State->bRolling)
		{
			elapsed += DeltaSeconds;
			if(elapsed > 0.09f)
			{
				RollDiceContinuedBP();
				State->MyRoll = FMath::RandRange(1, 10);
				//UpdateRoll();
				elapsed = 0;
			}
		}
	}


	if (IsValid(State))
	{
		State->dt = DeltaSeconds;
		State->UpdateState();
	}
}

void ACoreLocalPlayerController::RollDiceBegin()
{
	if (State->bRolled || !State->bIsMyTurn || !State->bRollMode)
		return;

	State->bRolling = true;
	State->bRollMode = true;
	State->MyRoll = FMath::RandRange(1, 10);
	RollDiceBeginBP();

}

void ACoreLocalPlayerController::RollDiceComplete()
{
	if (!State->bIsMyTurn)
		return;

	if (State->MyRoll <= 0)
	{
		PawnArrived(State->myPawn->BoardSpace);
	}
	if (!State->myPawn->OnPawnArrivedAtNewSpace.IsBoundToObject(this))
		State->myPawn->OnPawnArrivedAtNewSpace.BindUObject(this, &ACoreLocalPlayerController::PawnArrived);
	State->bPostRollPreMove = false;
	if (State->myPawn->BoardSpace->HasMultiplePaths(1))
	{
		ActivatePathSelect(*State->myPawn->BoardSpace);
		return;
	}
	ContinueMovement();
}


void ACoreLocalPlayerController::CancelActivated()
{
	if (!State->bIsMyTurn)
		return;

	if (DeclineStack.Num() > 0)
	{
		DeclineStack[0]();
		DeclineStack.Empty();
		return;
	}

	if(State->bRolling)
	{
		State->bRolling = false;
		RollDiceCancelledBP();
	}

	if (State->bRollMode)
	{
		State->bRollMode = false;
	}

	if (State->bCameraMode)
	{
		State->bCameraMode = false;
		State->TurnCharacter->bFreeCameraMode = false;
	}
	State->TurnCharacter->DefaultMode();
}

void ACoreLocalPlayerController::ActivatePathSelect(const ABoardSpace& space)
{
	State->bSelectingPaths = true;
	State->myPawn->DisplayPaths(State->MyRoll);
	CurrentPathIndex = 0;
	MaxPathIndex = space.NextTiles.Num()-1;

	PathDirections.Empty();
	for(const auto _space : space.NextTiles)
	{
		FVector dir = _space->GetActorLocation() - space.GetActorLocation();
		dir.Z = 0;
		dir.Normalize();
		PathDirections.Add(FVector2D{dir.X, dir.Y});
	}

	// Logic when A is Pressed to accept path
	const TFunction<void(int)> SelectedPathLogic = [this](int path)
	{
		State->bSelectingPaths = false;
		State->myPawn->HidePaths();
		State->myPawn->Move(path);
		if (State->bCameraMode)
		{
			State->TurnCharacter->bFreeCameraMode = false;
			State->bCameraMode = false;
			State->TurnCharacter->DefaultMode();
		}
		CurrentPathIndex = 0;
	};
	ConfirmStack.Add(SelectedPathLogic);
}


void ACoreLocalPlayerController::Confirm(const FInputActionValue& Value)
{
	if (!State->bIsMyTurn)
		return;
	const bool Clicked = Value.Get<bool>();

	if(!State->bRolled && Clicked && !State->bRollMode)
	{
		if (State->bCameraMode)
		{
			State->bCameraMode = false;
			State->TurnCharacter->bFreeCameraMode = false;
		}
		State->TurnCharacter->RollDiceModeActivated();
		State->bRollMode = true;
		return;
	}

	if(Clicked && ConfirmStack.Num() > 0)
	{
		ConfirmStack[0](CurrentPathIndex);
		ConfirmStack.Empty();
	}
}

void ACoreLocalPlayerController::ConfirmReleased(const FInputActionValue& Value)
{
	if(State->bIsMyTurn && !State->bRolled && State->bRolling && !State->bCameraMode)
	{
		State->bRolling = false;
		State->bRollMode = false;
		
		State->bCameraMode = false;
		State->TurnCharacter->bFreeCameraMode = State->bCameraMode;
		State->TurnCharacter->DefaultMode();
		State->bRolled = true;
		PlayerRolledDice();		
	}
}

void ACoreLocalPlayerController::CameraModeToggle(const FInputActionValue& Value)
{
	if(State->bIsMyTurn && Value.Get<bool>() == true && !State->bRolling )
	{
		if (State->myPawn->IsMoving())
			State->bCameraMode = false;
		else
			State->bCameraMode = !State->bCameraMode;

		if (State->bCameraMode)
			State->TurnCharacter->LookAroundMode();
		else
			State->TurnCharacter->DefaultMode();

		State->TurnCharacter->bFreeCameraMode = State->bCameraMode;

		if (State->bRollMode && !State->bRolling)
			State->bRollMode = false;
	}
}

void ACoreLocalPlayerController::SelectPathHorizontal(const FInputActionValue& Value)
{
	if(!State->bIsMyTurn)
		return;

	if (State->bSelectingPaths)
	{
		SelectPathFromDirection(FVector2D(0, Value.Get<float>()));
	}

}

void ACoreLocalPlayerController::SelectPathVertical(const FInputActionValue& Value)
{
	if (!State->bIsMyTurn)
		return;
	if(State->bSelectingPaths)
		SelectPathFromDirection(FVector2D(Value.Get<float>(), 0));

}

void ACoreLocalPlayerController::JoystickInput(const FInputActionValue& Value)
{
	if (!State->bIsMyTurn || !IsValid(State->TurnCharacter))
		return;

	const FVector2D input = Value.Get<FVector2D>();

	if (State->bCameraMode && input.Size() > 0.f)
		State->TurnCharacter->MoveCamera(input);

	else if (State->bSelectingPaths && input.Size() > 0.2f)
	{
		FVector2D normalizedInput = FVector2D(input.Y, input.X);
		normalizedInput = normalizedInput.GetSafeNormal();
		SelectPathFromDirection(normalizedInput);
	}
	/*else if (State->bSelectingShrine)
	{
		FVector2D normalizedInput = FVector2D(input.X, input.Y);
		if (normalizedInput.X < 0.4f && normalizedInput.X > -0.4f && !bRightJoystickReset)
		{
			bRightJoystickReset = true;
		}
		SelectShrineFromDirection(normalizedInput);
	}*/
}

void ACoreLocalPlayerController::SelectPathFromDirection(FVector2D Direction)
{
	Direction.Normalize();
	Direction = Direction.GetRotated(State->TurnCharacter->GetActorRotation().Euler().Z);
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
		State->myPawn->UpdatePaths(CurrentPathIndex);
	}
}

/*void ACoreLocalPlayerController::SelectShrineFromDirection(FVector2D Direction)
{
	if(bRightJoystickReset)
	{
		if (Direction.X > 0.2f && CurrentPathIndex < MaxPathIndex)
		{
			CurrentPathIndex += 1;
			bRightJoystickReset = false;
			State->myPawn->UpdateShrineOptions(CurrentPathIndex);
		}
		else if( Direction.X < -0.2f && CurrentPathIndex > 0)
		{
			CurrentPathIndex -= 1;
			bRightJoystickReset = false;
			State->myPawn->UpdateShrineOptions(CurrentPathIndex);
		}
	}
}*/

/*
void ACoreLocalPlayerController::StartSelectingShrineOptions()
{
	if (!State->bIsMyTurn)
		return;

	State->bSelectingShrine = true;
	CurrentPathIndex = 1;
	MaxPathIndex = 2;
	State->myPawn->DisplayShrineOptions();

	const TFunction<void(int)> ConfirmShrineLogic = [&](int i)
	{
		State->bSelectingShrine = false;
		State->myPawn->HideShrineOptions();
		ContinueMovement();
		State->Coins -= 5;
		GEngine->AddOnScreenDebugMessage(590482094, 5.f, FColor::Red, FString("Confirmed SHRINE"));
		ConfirmStack.Empty();
		DeclineStack.Empty();
	};
	const TFunction<void()> DeclineShrineLogic = [&]()
	{
		State->bSelectingShrine = false;
		State->myPawn->HideShrineOptions();
		ContinueMovement();
		GEngine->AddOnScreenDebugMessage(590482094, 5.f, FColor::Red, FString("Declined SHRINE"));
		ConfirmStack.Empty();
		DeclineStack.Empty();
	};
	ConfirmStack.Add(ConfirmShrineLogic);
	DeclineStack.Add(DeclineShrineLogic);

}
*/
