// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h" // Put new includes above

// Forward declarations
class UTankBarrel;
class UTankTurrent;
class UTankAimingComponent;
class AProjectile;

UCLASS()
class S04_BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:

	UFUNCTION(Blueprintcallable, Category = Setup)
	void SetBarrelReference(UTankBarrel* BarrelToSet);

	UFUNCTION(Blueprintcallable, Category = Setup)
	void SetTurrentReference(UTankTurrent* TurrentToSet);

	UFUNCTION(Blueprintcallable, Category = Firing)
	void Fire();

	void AimAt(FVector HitLocation);

protected:
	UTankAimingComponent* TankAimingComponent = nullptr;

private:
	// Sets default values for this pawn's properties
	ATank();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, category = Firing)
	float LaunchSpeed = 8000.0f;

	UPROPERTY(EditAnywhere, category = Setup)
	TSubclassOf<AProjectile> ProjectileBlueprint;

	// Local barrel reference for spawning projectile
	UTankBarrel* Barrel = nullptr;
	
};
