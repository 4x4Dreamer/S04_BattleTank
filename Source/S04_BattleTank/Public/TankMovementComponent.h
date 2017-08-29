// Copyright 2017 memtrade.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "TankMovementComponent.generated.h"

class UTankTrack;

/**
 * Responsible for driving the tank tracks
 */
UCLASS( ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class S04_BATTLETANK_API UTankMovementComponent : public UNavMovementComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, category = "Setup")
	void Initialise(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet);

	UFUNCTION(BlueprintCallable, category = "Input")
	void IntendMoveForward(float Throw);

	UFUNCTION(BlueprintCallable, category = "Input")
	void IntendTurnRight(float Throw);

private:
	// Called from the pathfinding logic by the AI controller
	virtual void RequestDirectMove(const FVector& MoveVelocity, bool ForceMaxSpeed) override;

	UTankTrack* LeftTrack = nullptr;
	UTankTrack* RightTrack = nullptr;
};
