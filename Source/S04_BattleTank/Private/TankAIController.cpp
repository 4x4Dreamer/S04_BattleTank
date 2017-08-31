// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "S04_BattleTank.h"
#include "TankAimingComponent.h"
#include "TankAIController.h"
#include "Engine.h"
// Depends on movement component via pathfinding system

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto ControlledTank = GetPawn();

	if (!ensure(PlayerTank && ControlledTank)) { return; }
	
	// Move toward the player
	//   MovetoACtor() is inherited function of AAIController
	//   It calls RequestDirectMove (also an inhertied function) in TankMoveMentComponent
	MoveToActor(PlayerTank, AcceptanceRadius);  // TODO check if radius is in cm

	// Aim towards the player
	auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();

	AimingComponent->AimAt(PlayerTank->GetActorLocation());

	AimingComponent->Fire(); // limit firing rate done in tank
	
}
