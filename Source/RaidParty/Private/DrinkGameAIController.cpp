// Fill out your copyright notice in the Description page of Project Settings.


#include "DrinkGameAIController.h"

ADrinkGameAIController::ADrinkGameAIController()
{
	bWantsPlayerState = true;
}
void ADrinkGameAIController::InputNextTarget()
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