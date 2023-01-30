// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreLocalPlayerController.h"

ACoreLocalPlayerController::ACoreLocalPlayerController()
{
	bAutoManageActiveCameraTarget = false;
	//bFindCameraComponentWhenViewTarget = false;
}

void ACoreLocalPlayerController::BeginPlay()
{
	Super::BeginPlay();
	auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

}

void ACoreLocalPlayerController::OnPossess(APawn* aPawn)
{
	bAutoManageActiveCameraTarget = false;
	Super::OnPossess(aPawn);
}

void ACoreLocalPlayerController::OnUnPossess()
{
	bAutoManageActiveCameraTarget = false;
	Super::OnUnPossess();
}

