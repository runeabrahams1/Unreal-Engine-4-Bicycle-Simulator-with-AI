// Fill out your copyright notice in the Description page of Project Settings.

#include "BikeV3.h"
#include "MyWheeledVehicle.h"
#include "BikeV3WheelFront.h"
#include "BikeV3WheelRear.h"
#include "BikeMovementComponent.h"

AMyWheeledVehicle::AMyWheeledVehicle()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Our root component will be a sphere that reacts to physics
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BikeMesh(TEXT("/Game/Bike/bike_skeletal_meshV2.bike_skeletal_meshV2"));
	GetMesh()->SetSkeletalMesh(BikeMesh.Object);
	RootComponent = GetMesh();

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

	// Take control of the default player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Create an instance of our movement component, and tell it to update the root.
	OurMovementComponent = CreateDefaultSubobject<UBikeMovementComponent>(TEXT("BikeMovementComponent"));
	OurMovementComponent->UpdatedComponent = RootComponent;
}

void AMyWheeledVehicle::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &AMyWheeledVehicle::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMyWheeledVehicle::MoveRight);

	InputComponent->BindAction("Handbrake", IE_Pressed, this, &AMyWheeledVehicle::OnHandbrakePressed);
	InputComponent->BindAction("Handbrake", IE_Released, this, &AMyWheeledVehicle::OnHandbrakeReleased);

	InputComponent->BindAction("ShiftUp", IE_Pressed, this, &AMyWheeledVehicle::ShiftUp);
	InputComponent->BindAction("ShiftDown", IE_Pressed, this, &AMyWheeledVehicle::ShiftDown);

}

UBikeMovementComponent* AMyWheeledVehicle::GetVehicleMovementComponent() const
{
	return OurMovementComponent;
}

void AMyWheeledVehicle::MoveForward(float val)
{
	GetVehicleMovementComponent()->SetThrottleInput(val);
}

void AMyWheeledVehicle::MoveRight(float Val)
{
	GetVehicleMovementComponent()->SetSteeringInput(Val);
}

void AMyWheeledVehicle::ShiftUp()
{
	if (GetVehicleMovementComponent()->GetCurrentGear() < 5)
	{
		GetVehicleMovementComponent()->SetTargetGear(GetVehicleMovementComponent()->GetCurrentGear() + 1, true);
	}
}

void AMyWheeledVehicle::ShiftDown()
{
	if (GetVehicleMovementComponent()->GetCurrentGear() > -1)
	{
		GetVehicleMovementComponent()->SetTargetGear(GetVehicleMovementComponent()->GetCurrentGear() - 1, true);
	}
}

void AMyWheeledVehicle::OnHandbrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void AMyWheeledVehicle::OnHandbrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void AMyWheeledVehicle::Tick(float Delta)
{

}

void AMyWheeledVehicle::BeginPlay()
{

}