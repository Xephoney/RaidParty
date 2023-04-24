// Fill out your copyright notice in the Description page of Project Settings.


#include "HeadInMouthAIController.h"

AHeadInMouthAIController::AHeadInMouthAIController()
{
	bWantsPlayerState = true;
}

void AHeadInMouthAIController::BraveryCheck()
{
	if (FMath::RandRange(0.f, 100.f) < BraveryPercentage)
	{
		bHandInMouth = true;
	}
	else
	{
		bHandInMouth = false;
		BraveryPercentage += BraveryPercentage;
	}
}