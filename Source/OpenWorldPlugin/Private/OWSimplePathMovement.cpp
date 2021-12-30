// This file is part of OpenWorldPlugin. Copyright Bolotaev "m0nstr0" Sergey http://bolotaev.com/


#include "OWSimplePathMovement.h"

#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

UOWPathAssetNode* UOWSimplePathMovement::GetNextNode()
{
    return nullptr;
}

void UOWSimplePathMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    UMovementComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!PathAsset || !UpdatedComponent || ShouldSkipUpdate(DeltaTime)) {
		return;
	}

	ACharacter* ActorOwner = Cast<ACharacter>(UpdatedComponent->GetOwner());
	if (!ActorOwner) {
		return;
	}

	UPawnMovementComponent* MovementComponent = ActorOwner->GetMovementComponent();
	if (!MovementComponent) {
		return;
	}

	if (UpdatedComponent->IsSimulatingPhysics()) {
		return;
	}

	UOWPathAssetNode* NextNode = GetNextNode();
	if (!NextNode) {
		return;
	}
	/*
	MovementComponent->
	SafeMoveUpdatedComponent(MoveDelta, CurrentRotation, bSweep, Hit, TeleportType);

	float RemainingTime = DeltaTime;
	int32 NumBounces = 0;
	int32 Iterations = 0;
	FHitResult Hit(1.f);

	while (RemainingTime >= MIN_TICK_TIME && (Iterations < MaxSimulationIterations) && IsValid(ActorOwner) && UpdatedComponent && IsActive()) {
		Iterations++;

		const float TimeTick = ShouldUseSubStepping() ? GetSimulationTimeStep(RemainingTime, Iterations) : RemainingTime;
		RemainingTime -= TimeTick;

		// Calculate the current alpha with this tick iteration
		const float TargetTime = FMath::Clamp(CurrentTime + ((TimeTick * TimeMultiplier) * CurrentDirection), 0.0f, 1.0f);
		FVector MoveDelta = ComputeMoveDelta(TargetTime);

		// Update velocity
		Velocity = MoveDelta / TimeTick;

		// Update the rotation on the spline if required
		FRotator CurrentRotation = UpdatedComponent->GetComponentRotation(); //-V595

		// Move the component
		if ((bPauseOnImpact == false) && (BehaviourType != EInterpToBehaviourType::OneShot)) {
			// If we can bounce, we are allowed to move out of penetrations, so use SafeMoveUpdatedComponent which does that automatically.
			SafeMoveUpdatedComponent(MoveDelta, CurrentRotation, bSweep, Hit, TeleportType);
		}
		else {
			// If we can't bounce, then we shouldn't adjust if initially penetrating, because that should be a blocking hit that causes a hit event and stop simulation.
			TGuardValue<EMoveComponentFlags> ScopedFlagRestore(MoveComponentFlags, MoveComponentFlags | MOVECOMP_NeverIgnoreBlockingOverlaps);
			MoveUpdatedComponent(MoveDelta, CurrentRotation, bSweep, &Hit, TeleportType);
		}
		//DrawDebugPoint(GetWorld(), UpdatedComponent->GetComponentLocation(), 16, FColor::White,true,5.0f);
		// If we hit a trigger that destroyed us, abort.
		if (!IsValid(ActorOwner) || !UpdatedComponent || !IsActive()) {
			return;
		}

		// Update current time
		float AlphaRemainder = 0.0f;
		if (bIsWaiting == false) {
			// Compute time used out of tick time to get to the hit
			const float TimeDeltaAtHit = TimeTick * Hit.Time;
			// Compute new time lerp alpha based on how far we moved
			CurrentTime = CalculateNewTime(CurrentTime, TimeDeltaAtHit, Hit, true, bStopped, AlphaRemainder);
		}

		// See if we moved at all
		if (Hit.Time != 0.f) {
			// If we were 'waiting' we are not any more - broadcast we are moving again
			if (bIsWaiting == true) {
				OnWaitEndDelegate.Broadcast(Hit, CurrentTime);
				bIsWaiting = false;
			}
		}

		// Handle hit result after movement
		float SubTickTimeRemaining = 0.0f;
		if (!Hit.bBlockingHit) {
			if (bStopped == true) {
				Velocity = FVector::ZeroVector;
				break;
			}

			// Handle remainder of alpha after it goes off the end, for instance if ping-pong is set and it hit the end,
			// continue with the time remaining off the end but in the reverse direction. It is similar to hitting an object in this respect.
			if (AlphaRemainder != 0.0f) {
				NumBounces++;
				SubTickTimeRemaining = (AlphaRemainder * Duration);
			}
		}
		else {
			if (HandleHitWall(Hit, TimeTick, MoveDelta)) {
				break;
			}

			NumBounces++;
			SubTickTimeRemaining = TimeTick * (1.f - Hit.Time);
		}

		// A few initial bounces should add more time and iterations to complete most of the simulation.
		if (NumBounces <= 2 && SubTickTimeRemaining >= MIN_TICK_TIME) {
			RemainingTime += SubTickTimeRemaining;
			Iterations--;
		}
	}

    UpdateComponentVelocity();*/
}
