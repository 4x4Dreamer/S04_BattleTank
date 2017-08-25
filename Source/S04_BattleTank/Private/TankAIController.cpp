// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "S04_BattleTank.h"
#include "Tank.h"
#include "Engine.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	auto ControlledTank = Cast<ATank>(GetPawn());

	if (PlayerTank)
	{
		// TODO Move toward the player

		// Aim towards the player
		ControlledTank->AimAt(PlayerTank->GetActorLocation());

		//ControlledTank->Fire(); // limit firing rate done in tank
	}
}