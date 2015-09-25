// Fill out your copyright notice in the Description page of Project Settings.

#include "BikeV3.h"
#include "Engine.h"
#include "CollidingPawn.h"
#include "CollidingPawnMovementComponent.h"
#include "BikeAnimInstance.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)


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

	// Create a particle system that we can activate or deactivate
	OurParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
	OurParticleSystem->AttachTo(RootComponent);
	OurParticleSystem->bAutoActivate = false;
	OurParticleSystem->SetRelativeLocation(FVector(-20.0f, 0.0f, 20.0f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));
	if (ParticleAsset.Succeeded())
	{
		OurParticleSystem->SetTemplate(ParticleAsset.Object);
	}

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
	TwistConstraint->SetAngularTwistLimit(ACM_Limited, 0);
	TwistConstraint->SetAngularOrientationDrive(true, true);
	TwistConstraint->SetAngularDriveParams(10, 10, 10);
	TwistConstraint->SetLinearXLimit(LCM_Free, 0);
	TwistConstraint->SetLinearYLimit(LCM_Free, 0);
	TwistConstraint->SetLinearZLimit(LCM_Free, 0);

	// Animation Instance
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> BikeAnimBP(TEXT("/Game/Bike/Bike_Anim_BP.Bike_Anim_BP_C"));
	BikeAnimation = Cast<UBikeAnimInstance>(BikeComponent->GetAnimInstance());
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
	FRotator newTarget = FRotator(BikeComponent->GetForwardVector().Y, BikeComponent->GetForwardVector().Z, 0);
	TwistConstraint->SetAngularOrientationTarget(newTarget);
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
	if (AxisValue != 0.f && BikeComponent->GetPhysicsLinearVelocity().Size() < 500)
	{
		FVector Direction = BikeComponent->GetForwardVector()*AxisValue*400;
		BikeComponent->AddImpulse(Direction, FName("PhyWheel_B"), true);
	}
}

void ACollidingPawn::MoveRight(float AxisValue)
{
	FRotator newTurn = BikeAnimation->SkelControl_WheelRot_F;
	newTurn.Yaw = AxisValue * 70;
	BikeAnimation->SkelControl_WheelRot_F = newTurn;
}

void ACollidingPawn::Turn(float AxisValue)
{
}

void ACollidingPawn::ParticleToggle()
{
	if (OurParticleSystem && OurParticleSystem->Template)
	{
		OurParticleSystem->ToggleActive();
	}
}

