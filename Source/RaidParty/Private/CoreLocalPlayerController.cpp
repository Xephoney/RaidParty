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
	
	ArrivedAtSpace(space);
	
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
		/*
		switch (space->Type)
		{
		case SPACETYPE::NORMAL: 
			ContinueMovement();
			break;
		case SPACETYPE::BAD: 
			ContinueMovement();
			break;
		case SPACETYPE::EVENT: 
			break;
		case SPACETYPE::SHRINE: 
			break;
		case SPACETYPE::KEEP: 
			break;
		default: ;
		}
		*/

		PlayerHalted(space->Type);
		return;
	}
	myPawn->Move();

}

void ACoreLocalPlayerController::ContinueMovement()
{
	if (MyRoll > 0)
		myPawn->Move();
	else
		EndTurn();
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
		DebugLog += "| Select Paths  | " + FString::Printf(TEXT("%s\n"), bSelectingPaths ? TEXT("true") : TEXT("false"));
		DebugLog += "| Select Shrine | " + FString::Printf(TEXT("%s\n"), bSelectingShrine ? TEXT("true") : TEXT("false"));
		DebugLog += "| Rolled        | " + FString::Printf(TEXT("%s\n"), bRolled ? TEXT("true") : TEXT("false"));
		DebugLog += "| Rolling       | " + FString::Printf(TEXT("%s\n"), bRolling ? TEXT("true") : TEXT("false"));
		DebugLog += "| Roll Mode     | " + FString::Printf(TEXT("%s\n"), bRollMode ? TEXT("true") : TEXT("false"));
		DebugLog += "| Camera Mode   | " + FString::Printf(TEXT("%s\n"), bCameraMode ? TEXT("true") : TEXT("false"));
		DebugLog += "| Confirm Stack | [" + FString::FromInt(ConfirmStack.Num()) + "]\n";
		DebugLog += "| Decline Stack | [" + FString::FromInt(DeclineStack.Num()) + "]\n";
		if (bSelectingPaths || bSelectingShrine)
			DebugLog += "| Current Index/Max Index " + FString::FromInt(CurrentPathIndex) + " / " + FString::FromInt(MaxPathIndex);

		GEngine->AddOnScreenDebugMessage(675946584, 0.5f, FColor::Purple, DebugLog);

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
	if (DeclineStack.Num() > 0)
	{
		DeclineStack[0]();
		DeclineStack.RemoveAt(0);
	}

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
	bSelectingShrine = false;
	myPawn->DisplayPaths(MyRoll);
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
	const TFunction<void()> SelectedPathLogic = [this]()
	{
		bSelectingPaths = false;
		myPawn->HidePaths();
		myPawn->Move(CurrentPathIndex);
		GEngine->AddOnScreenDebugMessage(590482094, 5.f, FColor::Red, FString("Confirmed PATH"));
		CurrentPathIndex = 0;
	};
	ConfirmStack.Add(SelectedPathLogic);
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
		return;
	}

	if(Clicked && ConfirmStack.Num() > 0)
	{
		ConfirmStack[0]();
		ConfirmStack.Empty();
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
	if(bIsMyTurn && !bSelectingShrine && Value.Get<bool>() == true)
	{
		bCameraMode = !bCameraMode;
		TurnCharacter->bFreeCameraMode = bCameraMode;
	}
}

void ACoreLocalPlayerController::SelectPathHorizontal(const FInputActionValue& Value)
{
	if(!bIsMyTurn)
		return;

	if (bSelectingPaths)
	{
		SelectPathFromDirection(FVector2D(0, Value.Get<float>()));
		return;
	}
	if(bSelectingShrine)
	{
		SelectShrineFromDirection(FVector2D(Value.Get<float>(), 0));
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
	
	if (bCameraMode && input.Size() > 0.f)
		TurnCharacter->MoveCamera(input);

	else if (bSelectingPaths && input.Size() > 0.2f)
	{
		FVector2D normalizedInput = FVector2D(input.Y, input.X);
		normalizedInput = normalizedInput.GetSafeNormal();
		SelectPathFromDirection(normalizedInput);
	}
	else if (bSelectingShrine && input.Size() > 0.2f)
	{
		FVector2D normalizedInput = FVector2D(input.X, input.Y);
		normalizedInput = normalizedInput.GetSafeNormal();
		SelectShrineFromDirection(normalizedInput);
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
		myPawn->UpdatePaths(CurrentPathIndex);
	}
}

void ACoreLocalPlayerController::SelectShrineFromDirection(FVector2D Direction)
{
	if (Direction.X > 0.1f && CurrentPathIndex < MaxPathIndex)
	{
		CurrentPathIndex += 1;
	}
	else if( Direction.X < -0.1f && CurrentPathIndex > 0)
	{
		CurrentPathIndex -= 1;
	}

	myPawn->UpdateShrineOptions(CurrentPathIndex);
}

void ACoreLocalPlayerController::StartSelectingShrineOptions()
{
	if (!bIsMyTurn)
		return;

	bSelectingShrine = true;
	CurrentPathIndex = 1;
	MaxPathIndex = 2;
	myPawn->DisplayShrineOptions();

	const TFunction<void()> ConfirmShrineLogic = [this]()
	{
		bSelectingShrine = false;
		myPawn->HideShrineOptions();
		ContinueMovement();
		State->Coins -= 5;
		GEngine->AddOnScreenDebugMessage(590482094, 5.f, FColor::Red, FString("Confirmed SHRINE"));
		ConfirmStack.Empty();
		DeclineStack.Empty();
	};
	const TFunction<void()> DeclineShrineLogic = [this]()
	{
		bSelectingShrine = false;
		myPawn->HideShrineOptions();
		ContinueMovement();
		GEngine->AddOnScreenDebugMessage(590482094, 5.f, FColor::Red, FString("Declined SHRINE"));
		ConfirmStack.Empty();
		DeclineStack.Empty();
	};
	ConfirmStack.Add(ConfirmShrineLogic);
	DeclineStack.Add(DeclineShrineLogic);

}
