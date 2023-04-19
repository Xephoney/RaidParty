// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerBase.h"

AAIControllerBase::AAIControllerBase()
{
	bWantsPlayerState = true;
}

void AAIControllerBase::InputNextTarget()
{
	if (TargetInputQueue.Num() > 0)
	{
		DirectionalInput = TargetInputQueue[InputIterator];
		InputIterator++;
		Points++;
		if (InputIterator == TargetInputQueue.Num())
			bLastInput = true;
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Nothing in queue!"));
}

void AAIControllerBase::BraveryCheck()
{
	if(FMath::RandRange(0.f,100.f)<BraveryPercentage)
	{
		bHandInMouth = true;
	}
	else
	{
		bHandInMouth = false;
		BraveryPercentage += BraveryPercentage;
	}
}
