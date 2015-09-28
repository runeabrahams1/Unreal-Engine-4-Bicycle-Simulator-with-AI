// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "CollidingPawn.generated.h"
UCLASS(config=Game)
class BIKEV3_API ACollidingPawn : public APawn
{
	GENERATED_BODY()

	UPROPERTY(Category = Root, VisibleDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* BikeComponent;

	UPROPERTY(Category = Constraints, EditAnywhere, meta = (AllowPrivateAcces = "true"))
	UPhysicsConstraintComponent* TwistConstraint;

public:
	// Sets default values for this pawn's properties
	ACollidingPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UParticleSystemComponent *OurParticleSystem;

	class UCollidingPawnMovementComponent* OurMovementComponent;

	virtual UPawnMovementComponent* GetMovementComponent() const override;

	UFUNCTION()
	void MoveForward(float AxisValue);
	UFUNCTION()
	void MoveRight(float AxisValue);
	UFUNCTION()
	void Turn(float AxisValue);
	UFUNCTION()
	void ParticleToggle();
	
};
