// Fill out your copyright notice in the Description page of Project Settings.

#include "BikeV3.h"
#include "WheeledBike.h"
#include "BikeMovementComponent.h"
#include "BikeV3WheelFront.h"
#include "BikeV3WheelRear.h"
#include "BikeAnimInstance.h"

#define GEAR_MAX 5
#define GEAR_NEUTRAL 0
#define GEAR_REVERSE -1
#define MAX_SPEED 500
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

AWheeledBike::AWheeledBike(const FObjectInitializer &initilaizer) : Super(initilaizer.SetDefaultSubobjectClass<UBikeMovementComponent>(VehicleMovementComponentName))
{
	// Bike mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BikeMesh(TEXT("/Game/Bike/bike_skeletal_meshV2.bike_skeletal_meshV2"));
	GetMesh()->SetSkeletalMesh(BikeMesh.Object);

	//// Animation Instance
	//static ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass> BikeAnimBP(TEXT("/Game/Bike/BikeAnimBP2.BikeAnimBP2_C"));
	//GetMesh()->AnimBlueprintGeneratedClass = BikeAnimBP.Object;

	// Take control of the default player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	GetMesh()->SetAllBodiesSimulatePhysics(true);

	UBikeMovementComponent* BikeMovement = CastChecked<UBikeMovementComponent>(GetVehicleMovement());

	check(BikeMovement->WheelSetups.Num() == 2);

	BikeMovement->WheelSetups[0].WheelClass = UBikeV3WheelFront::StaticClass();
	BikeMovement->WheelSetups[0].BoneName = FName("PhyWHeel_F");
	BikeMovement->WheelSetups[0].AdditionalOffset = FVector(0.f, 0.f, 0.f);

	BikeMovement->WheelSetups[1].WheelClass = UBikeV3WheelRear::StaticClass();
	BikeMovement->WheelSetups[1].BoneName = FName("PhyWheel_B");
	BikeMovement->WheelSetups[1].AdditionalOffset = FVector(0.f, 0.f, 0.f);

	// Adjust the tire loading
	BikeMovement->MinNormalizedTireLoad = 0.0f;
	BikeMovement->MinNormalizedTireLoadFiltered = 0.2f;
	BikeMovement->MaxNormalizedTireLoad = 2.0f;
	BikeMovement->MaxNormalizedTireLoadFiltered = 2.0f;

	// Physics settings
	// Adjust the center of mass, find a good position
	UPrimitiveComponent* UpdatedPrimitive = Cast<UPrimitiveComponent>(BikeMovement->UpdatedComponent);
	if (UpdatedPrimitive)
	{
		UpdatedPrimitive->BodyInstance.COMNudge = FVector(0.0f, 0.0f, 0.0f);
	}

	// Set the inertia scale. This controls how the mass of the vehicle is distributed.
	BikeMovement->InertiaTensorScale = FVector(1.0f, 1.0f, 1.0f);

	// Physics Constraint
	TwistConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("TwistConstrant"));
	TwistConstraint->ConstraintActor1 = RootComponent->GetAttachmentRootActor();
	TwistConstraint->SetAngularTwistLimit(ACM_Limited, 0);
	TwistConstraint->SetAngularOrientationDrive(true, true);
	TwistConstraint->SetAngularDriveParams(10, 10, 10);
	TwistConstraint->SetLinearXLimit(LCM_Free, 0);
	TwistConstraint->SetLinearYLimit(LCM_Free, 0);
	TwistConstraint->SetLinearZLimit(LCM_Free, 0);

	// Create a spring arm component for our chase camera
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetRelativeLocation(FVector(-50.0f, 0.0f, 254.0f));
	SpringArm->SetWorldRotation(FRotator(-20.0f, 0.0f, 0.0f));
	SpringArm->AttachTo(RootComponent);
	SpringArm->TargetArmLength = 128.0f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->bEnableCameraRotationLag = false;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = true;

	// Create the chase camera component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ChaseCamera"));
	Camera->AttachTo(SpringArm, USpringArmComponent::SocketName);
	Camera->SetRelativeRotation(FRotator(10.0f, 0.0f, 0.0f));
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 90.f;

}

void AWheeledBike::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!GetMesh()) return;
	UBikeAnimInstance * Anim = Cast<UBikeAnimInstance>(GetMesh()->GetAnimInstance());
	if (!Anim) return;
	FRotator newTurnF = Anim->SkelControl_WheelRot_F;
	FRotator newTurnB = Anim->SkelControl_WheelRot_B;
	if (GetMesh()->GetForwardVector().X > 0)
	{
		newTurnF.Pitch -= RootComponent->GetComponentVelocity().Size() / 75.f; //GetVehicleMovement()->Wheels[0]->ShapeRadius; 
		newTurnB.Pitch -= RootComponent->GetComponentVelocity().Size() / 75.f; // GetVehicleMovement()->Wheels[1]->ShapeRadius;
	}
	else
	{
		newTurnF.Pitch += RootComponent->GetComponentVelocity().Size() / 75.f; // GetVehicleMovementComponent()->Wheels[0]->ShapeRadius;
		newTurnB.Pitch += RootComponent->GetComponentVelocity().Size() / 75.f; // GetVehicleMovementComponent()->Wheels[1]->ShapeRadius;
	}
	Anim->SkelControl_WheelRot_F = newTurnF;
	Anim->SkelControl_WheelRot_B = newTurnB;

	FRotator newTarget = FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw, 0);
	TwistConstraint->SetAngularOrientationTarget(newTarget);
	//GetVehicleMovementComponent()->TickVehicle(DeltaTime);
}

// Called when the game starts or when spawned
void AWheeledBike::BeginPlay()
{
	Super::BeginPlay();

}

void AWheeledBike::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAxis("MoveForward", this, &AWheeledBike::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AWheeledBike::MoveRight);
	InputComponent->BindAction("Handbrake", IE_Pressed, this, &AWheeledBike::OnHandbrakePressed);
	InputComponent->BindAction("Handbrake", IE_Released, this, &AWheeledBike::OnHandbrakeReleased);
	InputComponent->BindAction("ShiftUp", IE_Pressed, this, &AWheeledBike::ShiftUp);
	InputComponent->BindAction("ShiftDown", IE_Pressed, this, &AWheeledBike::ShiftDown);
}

void AWheeledBike::MoveForward(float AxisValue)
{
	GetVehicleMovementComponent()->SetThrottleInput(AxisValue);
	/*if (AxisValue != 0.f && GetMesh()->GetPhysicsLinearVelocity().Size() < 500)
	{
		FVector Direction = GetMesh()->GetForwardVector()*FMath::Clamp(AxisValue, 0.f, 1.f) * 400;
		GetMesh()->AddImpulse(Direction, FName("PhyWheel_B"), true);
	}*/
	AddActorLocalOffset(GetActorForwardVector()*AxisValue * 100);
}

void AWheeledBike::MoveRight(float Val)
{
	GetVehicleMovementComponent()->SetSteeringInput(Val);
}

void AWheeledBike::ShiftUp()
{
	if (GetVehicleMovementComponent()->GetCurrentGear() < GEAR_MAX)
	{
		GetVehicleMovementComponent()->SetTargetGear(GetVehicleMovementComponent()->GetCurrentGear() + 1, true);
	}
}

void AWheeledBike::ShiftDown()
{
	if (GetVehicleMovementComponent()->GetCurrentGear() > GEAR_REVERSE)
	{
		GetVehicleMovementComponent()->SetTargetGear(GetVehicleMovementComponent()->GetCurrentGear() - 1, true);
	}
}

void AWheeledBike::OnHandbrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void AWheeledBike::OnHandbrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}


