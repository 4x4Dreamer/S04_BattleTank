// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTurrent.h"

void UTankTurrent::Rotate(float RelativeSpeed)
{
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1.0, +1.0); // possible change (from 1.0 to 0.5) to stop barrel "jitters"
	auto RotationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto Rotation = RelativeRotation.Yaw + RotationChange;
	SetRelativeRotation(FRotator(0, Rotation, 0));
}


