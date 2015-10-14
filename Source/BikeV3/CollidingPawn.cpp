// Fill out your copyright notice in the Description page of Project Settings.

#include "BikeV3.h"
#include "Engine.h"
#include "CollidingPawn.h"
#include "CollidingPawnMovementComponent.h"
#include "BikeAnimInstance.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)
#define MAX_TURN 70


// Sets default values
ACollidingPawn::ACollidingPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Our root component will be a sphere that reacts to physics
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BikeMesh(TEXT("/Game/Bike/bike_skeletal_meshV2.bike_skeletal_meshV2"));
	BikeComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RootComponent"));
	BikeComponent->SetSkeletalMesh(BikeMesh.Object);
	BikeComponent->SetSimulatePhysics(true);
	BikeComponent->SetCollisionProfileName(TEXT("Pawn"));
	SetRootComponent(BikeComponent);

	// Use a spring arm to give the camera smooth, natural-feeling motion.
	USpringArmComponent* SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->AttachTo(RootComponent);
	SpringArm->RelativeLocation = FVector(0, 0, 128);
	SpringArm->RelativeRotation = FRotator(-45.f, 0.f, 0.f);
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.0f;

	// Create a camera and attach to our spring arm
	UCameraComponent* Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	Camera->AttachTo(SpringArm, USpringArmComponent::SocketName);
	Camera->RelativeRotation = FRotator(20.f, 0.f, 0.f);

	// Take control of the default player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Physics Constraint
	TwistConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("TwistConstrant"));
	TwistConstraint->ConstraintActor1 = RootComponent->GetAttachmentRootActor();
	TwistConstraint->SetWorldLocation(BikeComponent->GetComponentLocation());
	TwistConstraint->SetAngularTwistLimit(ACM_Limited, 0);
	TwistConstraint->SetAngularOrientationDrive(false, true);
	TwistConstraint->SetAngularDriveParams(1, 1, 1);
	TwistConstraint->SetLinearXLimit(LCM_Free, 0);
	TwistConstraint->SetLinearYLimit(LCM_Free, 0);
	TwistConstraint->SetLinearZLimit(LCM_Free, 0);
	BikeComponent->SetPhysicsMaxAngularVelocity(50, false);

	// Animation Instance
	static ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass> BikeAnimBP(TEXT("/Game/Bike/BikeAnimBP.BikeAnimBP_C"));
	BikeComponent->AnimBlueprintGeneratedClass = BikeAnimBP.Object;

	
}

UPawnMovementComponent* ACollidingPawn::GetMovementComponent() const
{
	return OurMovementComponent;
}

// Called when the game starts or when spawned
void ACollidingPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollidingPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

// Called to bind functionality to input
void ACollidingPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &ACollidingPawn::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ACollidingPawn::MoveRight);
	InputComponent->BindAction("ParticleToggle",IE_Pressed, this, &ACollidingPawn::ParticleToggle);
	InputComponent->BindAxis("Turn", this, &ACollidingPawn::Turn);

}

void ACollidingPawn::MoveForward(float AxisValue)
{
	int32 WheelFIdx = BikeComponent->GetPhysicsAsset()->FindBodyIndex(FName("PhyWHeel_F"));
	if (WheelFIdx >= 0 && AxisValue != 0.f && BikeComponent->GetPhysicsLinearVelocity().Size() < 500)
	{
		FBodyInstance* wheelF = BikeComponent->Bodies[WheelFIdx];
		// hack to apply force in the right direction regardless for actor rotation.
		if (FMath::IsWithin(wheelF->GetUnrealWorldTransform().GetRotation().Z, -90.f, 90.f))
		{
			wheelF->AddAngularImpulse(FVector(0, 20 * FMath::Clamp(0.f, 1.f, AxisValue), 0), true);
		}
		else
		{
			wheelF->AddAngularImpulse(FVector(0, 20 * FMath::Clamp(0.f, 1.f, AxisValue), 0), true);
		}
		//TODO Use set angualr velocity based on speed of physical wheel
	}

}

void ACollidingPawn::MoveRight(float AxisValue)
{
	// Get animation instance
	if (!BikeComponent) return;
	UBikeAnimInstance * Anim = Cast<UBikeAnimInstance>(BikeComponent->GetAnimInstance());
	if (!Anim) return;

	//Update front wheel
	int32 WheelFIdx = BikeComponent->GetPhysicsAsset()->FindBodyIndex(FName("PhyWHeel_F"));
	if (WheelFIdx >= 0)
	{
		//Update physics asset
		FBodyInstance* WheelF = BikeComponent->Bodies[WheelFIdx];
		FTransform wheelTrans = WheelF->GetUnrealWorldTransform();
		FRotator rot = wheelTrans.Rotator();
		rot.Yaw += AxisValue * MAX_TURN;
		wheelTrans.SetRotation(rot.Quaternion());
		WheelF->SetBodyTransform(wheelTrans, false);
		//Update animation
		Anim->SkelControl_WheelRot_F = WheelF->GetUnrealWorldTransform().Rotator();
	}
	//Update back wheel
	int32 WheelBIdx = BikeComponent->GetPhysicsAsset()->FindBodyIndex(FName("PhyWheel_B"));
	if (WheelBIdx >= 0)
	{
		//Update animation
		Anim->SkelControl_WheelRot_B = BikeComponent->Bodies[WheelBIdx]->GetUnrealWorldTransform().Rotator();
	}
}

void ACollidingPawn::Turn(float AxisValue)
{
}

void ACollidingPawn::ParticleToggle()
{
}

