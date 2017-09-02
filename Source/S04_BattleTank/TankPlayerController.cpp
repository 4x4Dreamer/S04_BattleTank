// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "S04_BattleTank.h"
#include "TankAimingComponent.h"
#include "Tank.h"
#include "Engine.h"

void ATankPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) { return; }

		// Subscribe our local method to the tank's death event
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnPossedTankDeath);
	}
}

void ATankPlayerController::OnPossedTankDeath()
{
	StartSpectatingOnly();
}

void ATankPlayerController::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("TankPlayerController C++ BeginPlay"));
	Super::BeginPlay();
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }
	FoundAimingComponent(AimingComponent);
}

void ATankPlayerController::Tick(float DeltaTime)
{
	//UE_LOG(LogTemp, Warning, TEXT("PlayerController is ticking"))
	Super::Tick(DeltaTime);

	AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetPawn()) { return; }  // e.g. if not possessing
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }

	FVector HitLocation; // Out parameter
	bool bGotHitLocation = GetSightRayHitLocation(HitLocation);
	if (bGotHitLocation) // has "side-effect", is going to trace line
	{
		//UE_LOG(LogTemp, Warning, TEXT("PlayerController calling AimingComponent->AimAt(HitLocation)"))
		AimingComponent->AimAt(HitLocation);
	}
}

// Get world location if linetrace through crosshair
bool ATankPlayerController::GetSightRayHitLocation(FVector& Hitlocation) const
{
	//Find the crosshair position in pixel coordinants
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY); // these are OUT parameters
	auto ScreenLocation = FVector2D(ViewportSizeX * CrosshairXLocation, ViewportSizeY * CrosshairYLocation);

	//"De-project" the screen position of the crosshair to a world direction
	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection))
	{
		// Line-trace along that look direction, and see what we hit (up to max range)
		return GetLookVectorHitLocation(LookDirection, Hitlocation);
	}
	return false;
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * LineTraceRange);
	if (GetWorld()->LineTraceSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			ECollisionChannel::ECC_Camera)
		)
	{
		HitLocation = HitResult.Location;
		return true;
	}
	HitLocation = FVector(0);
	return false;

}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
	FVector CameraWorldLocation; // to be discarded
	return DeprojectScreenPositionToWorld(
		ScreenLocation.X,
		ScreenLocation.Y,
		CameraWorldLocation,
		LookDirection
	);
}
