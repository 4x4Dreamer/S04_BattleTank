// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurrent.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include <string>
#include <iostream>

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();  // I added this

	UE_LOG(LogTemp, Warning, TEXT("Aiming Comp BeginPlay"))
	// So that first fire is after reload
	LastFireTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::Initialise(UTankBarrel* BarrelToSet, UTankTurrent* TurrentToSet)
{
	UE_LOG(LogTemp, Warning, TEXT("Aiming Comp Init"))
	Barrel = BarrelToSet;
	Turrent = TurrentToSet;
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	//char* test = "not set\0";
	//auto test = "not set\0";

	//auto test = std::string("not set");

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction); // added by me

	//UE_LOG(LogTemp, Warning, TEXT("Aiming Comp Ticking")) // YES !!!
	if ((FPlatformTime::Seconds() - LastFireTime) < ReLoadTimeInSeconds)
	{
		FiringState = EFiringState::Reloading;
		UE_LOG(LogTemp, Warning, TEXT("Firing State: Reloading"))
	}
	else
	if (IsBarrelMoving())
	{
		FiringState = EFiringState::Aiming;
		UE_LOG(LogTemp, Warning, TEXT("Firing State: Aiming"))
	}
	else
	{
		FiringState = EFiringState::Locked;
		UE_LOG(LogTemp, Warning, TEXT("Firing State: Locked"))
	}

	//char* test = std::string(FiringState);
	//std::cout << "Test\n";

	//UE_LOG(LogTemp, Warning, TEXT("Firing State: %s"), *test)
}

bool  UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) { return false; }
	auto BarrelForward = Barrel->GetForwardVector();
	return !BarrelForward.Equals(AimDirection, 0.01); // vectors are equal
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Barrel)) { return; }
	
	FVector OutLaunchVelocity = FVector(0.0f, 0.0f, 0.0f);
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity
	(
		this,
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace // Parameter must be present to prevent bug
	);

	if (bHaveAimSolution)
	{
		AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
	}
	// If no solution found do nothing
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	if (!ensure(Barrel) || !ensure(Turrent)) { return; };

	// Work out difference between current barrel rotation and AimDirection
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;

	//UE_LOG(LogTemp, Warning, TEXT("DeltaRotator: %s"), *DeltaRotator.ToString());

	// Always yaw the shortest way
	Barrel->Elevate(DeltaRotator.Pitch);
	if (FMath::Abs(DeltaRotator.Yaw) < 180)
	{
		Turrent->Rotate(DeltaRotator.Yaw);
	}
	else // Avoid going the long-way round
	{
		Turrent->Rotate(-DeltaRotator.Yaw);
	}

	// MYCHANGE: tried this check to minimize barrel "jitter" problen
	//if (!(abs(DeltaRotator.Yaw) < 1))
	//{
	//	Turrent->Rotate(DeltaRotator.Yaw);
	//}
}

void UTankAimingComponent::Fire()
{
	//UE_LOG(LogTemp, Warning, TEXT("HERE"))
	if (FiringState != EFiringState::Reloading)
	{
		//UE_LOG(LogTemp, Warning, TEXT("HERE2"))
		//Spawn a projectile at the socket location on the barrel
		if (!ensure(Barrel)) { return; }
		if (!ensure(ProjectileBlueprint)) { return; }
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile"))
			);

		Projectile->LaunchProjectile(LaunchSpeed);

		auto Time = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("%f: tank %s launched projectile at speed: %f"), Time, *GetName(), LaunchSpeed);

		LastFireTime = FPlatformTime::Seconds();
	}
}


