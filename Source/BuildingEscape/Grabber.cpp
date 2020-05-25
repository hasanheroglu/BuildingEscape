// Copyright Hasan Husamettin Eroglu 2020.

#include "Grabber.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

    FindPhysicsHandle();
    SetupInputComponent();
}

void UGrabber::Grab()
{
    UE_LOG(LogTemp, Warning, TEXT("Trying to grab."));

    FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
            OUT PlayerViewPointLocation,
            OUT PlayerViewPointRotation
    );

    FVector LineTraceDirection = PlayerViewPointRotation.Vector() * Reach;
    FVector LineTraceEnd = PlayerViewPointLocation + LineTraceDirection;


    FHitResult HitResult = GetFirstPhysicsBodyInReach();
    UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

    //If we hit something then attach the physics handle.
    if(HitResult.GetActor())
    {
        PhysicsHandle->GrabComponentAtLocation
        (
            ComponentToGrab,
            NAME_None,
            LineTraceEnd
        );
    }
}

void UGrabber::Release()
{
    UE_LOG(LogTemp, Warning, TEXT("Trying to release."));
    PhysicsHandle->ReleaseComponent();
}

// Checking for Physics Handle Component
void UGrabber::FindPhysicsHandle()
{
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (PhysicsHandle)
    {
        //PhysicsHandle found.
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("%s: Could not find PhysichHandleComponent."), *GetOwner()->GetName());
    }
}

void UGrabber::SetupInputComponent()
{
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

    if (InputComponent)
    {
        InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
        InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
    }
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
    FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
            OUT PlayerViewPointLocation,
            OUT PlayerViewPointRotation
    );

    FVector LineTraceDirection = PlayerViewPointRotation.Vector() * Reach;
    FVector LineTraceEnd = PlayerViewPointLocation + LineTraceDirection;
    
    FHitResult Hit;
    FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

    GetWorld()->LineTraceSingleByObjectType(
        Hit,
        PlayerViewPointLocation,
        LineTraceEnd,
        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
        TraceParams
    );

    AActor* ActorHit = Hit.GetActor();
    if (ActorHit)
    {
        UE_LOG(LogTemp, Warning, TEXT("Hit the %s"), *ActorHit->GetName()); 
    }

    return Hit;    
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
            OUT PlayerViewPointLocation,
            OUT PlayerViewPointRotation
    );

    FVector LineTraceDirection = PlayerViewPointRotation.Vector() * Reach;
    FVector LineTraceEnd = PlayerViewPointLocation + LineTraceDirection;

    if(PhysicsHandle->GrabbedComponent)
    {
        PhysicsHandle->SetTargetLocation(LineTraceEnd);
    }
}

