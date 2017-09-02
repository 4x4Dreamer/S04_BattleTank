// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "S04_BattleTank.h"
#include "TankAimingComponent.h"
#include "TankAIController.h"
#include "Tank.h" // So we can impliment OnDeath
#include "Engine.h"

// Depends on movement component via pathfinding system

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::SetPawn(APawn* InPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("AIController: Entering SetPawn")); // YES !!!

	Super::SetPawn(InPawn);
	UE_LOG(LogTemp, Warning, TEXT("AIController: After Super::SetPawn"));
	if (InPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController: Have InPawn")); // YES before play begins !!!

		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) { return; }

		UE_LOG(LogTemp, Warning, TEXT("AIController: Have Possessed Tank")); //  YES before play begins !!!

		// Subscribe our local method to the tank's death event
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossedTankDeath);
	}
}

void ATankAIController::OnPossedTankDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("AIController: Received"));  // YES !!!
	if (!ensure(GetPawn())) { return; } // TODO remove if ok
	UE_LOG(LogTemp, Warning, TEXT("AIController: Received2")); // YES !!!
	GetPawn()->DetachFromControllerPendingDestroy();
}

// Called every frame
void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto ControlledTank = GetPawn();

	if (!ensure(PlayerTank && ControlledTank)) { return; } // <---- ensure failure when player dies
	
	// Move toward the player
	//   MovetoACtor() is inherited function of AAIController
	//   It calls RequestDirectMove (also an inhertied function) in TankMoveMentComponent
	MoveToActor(PlayerTank, AcceptanceRadius);  // TODO check if radius is in cm

	// Aim towards the player
	auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
	AimingComponent->AimAt(PlayerTank->GetActorLocation());

	if (AimingComponent->GetFiringState() == EFiringState::Locked)
	{
		AimingComponent->Fire(); // limit firing rate done in tank
	}
}

