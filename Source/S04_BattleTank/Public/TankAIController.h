// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

// Forward declarations
class ATank;

/**
 * 
 */
UCLASS()
class S04_BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()
	
private:
	void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	// How close can the AI tank get to the player
	float AcceptanceRadius = 3000;

};
