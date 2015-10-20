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
	TwistConstraint->SetAngularDriveParams(5, 1, 200);
	TwistConstraint->SetAngularOrientationTarget(FRotator(0.f, 0.f, 0.f));
	TwistConstraint->SetLinearXLimit(LCM_Free, 0);
	TwistConstraint->SetLinearYLimit(LCM_Free, 0);
	TwistConstraint->SetLinearZLimit(LCM_Free, 0);

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
	//int32 WheelBIdx = BikeComponent->GetPhysicsAsset()->FindBodyIndex(FName("PhyWheel_B"));
	//if (WheelBIdx >= 0)
	//{
	//	FBodyInstance* wheelB = BikeComponent->Bodies[WheelBIdx];
	//	// hack to apply force in the right direction regardless for actor rotation.
	//	FVector wheelDir = FVector(1.f, 1.f, 1.f);
	//	wheelB->SetAngularVelocity(wheelDir * AxisValue * 50, true);
	//	if (FMath::IsWithin(wheelB->GetUnrealWorldTransform().Rotator().Pitch, 0.f, 180.f))
	//	{
	//	}
	//	//TODO Use set angualr velocity based on speed of physical wheel
	//}
	int32 WheelFIdx = BikeComponent->GetPhysicsAsset()->FindBodyIndex(FName("PhyWheel_F"));
	if (WheelFIdx >= 0)
	{
		FBodyInstance* wheelF = BikeComponent->Bodies[WheelFIdx];
		FVector wheelDir = FVector(0.f, 1.f, 0.f);
		wheelF->SetAngularVelocity(wheelDir * AxisValue * 500, false);
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
		FRotator rot = wheelTrans.GetRotation().Rotator();
		rot.Yaw +=  AxisValue *4;
		rot.Pitch = 0;
		rot.Roll = BikeComponent->GetComponentRotation().Roll;
		wheelTrans.SetRotation(rot.Quaternion());
		WheelF->SetBodyTransform(wheelTrans, ETeleportType::None);
		//Update animation
		Anim->SkelControl_WheelRot_F = rot;
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
	BikeComponent->AddImpulseAtLocation(FVector(0, 0, 10000),BikeComponent->GetBoneLocation(FName("PhyWHeel_F")));
}

