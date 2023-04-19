// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerBase.h"

void AAIControllerBase::InputNextTarget()
{
	if (TargetInputQueue.Num() > 0)
	{
		DirectionalInput = TargetInputQueue[InputIterator];
		InputIterator++;
		if (InputIterator == TargetInputQueue.Num())
			bLastInput = true;
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Nothing in queue!"));
}
