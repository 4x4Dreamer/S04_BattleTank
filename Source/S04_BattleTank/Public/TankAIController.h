// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

/**
 * 
 */
UCLASS()
class S04_BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

protected:
	// How close can the AI tank get to the player
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float AcceptanceRadius = 8000;
	
private:
	void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
};
