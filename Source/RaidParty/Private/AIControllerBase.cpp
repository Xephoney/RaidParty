// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerBase.h"

void AAIControllerBase::InputNextTarget()
{
	DirectionalInput = TargetInputQueue[InputIterator];
	InputIterator++;
}
