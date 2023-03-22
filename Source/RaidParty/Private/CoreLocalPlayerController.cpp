// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreLocalPlayerController.h"

#include "BoardPlayerState.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"

ACoreLocalPlayerController::ACoreLocalPlayerController()
{
	bAutoManageActiveCameraTarget = false;
	//bFindCameraComponentWhenViewTarget = false;
}

void ACoreLocalPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetShowMouseCursor(false);
	State = GetPlayerState<ABoardPlayerState>();
	if(!PlayerState)
		GEngine->AddOnScreenDebugMessage(53253, 10.f, FColor::Red, FString("FAILED TO GET PLAYER STATE"));
	
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
	
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

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
