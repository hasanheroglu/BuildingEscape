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
    FHitResult HitResult = GetFirstPhysicsBodyInReach();
    UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
    AActor* ActorHit = HitResult.GetActor();

    //If we hit something then attach the physics handle.
    if(ActorHit)
    {
        if (!PhysicsHandle) {return;}
        PhysicsHandle->GrabComponentAtLocation
        (
            ComponentToGrab,
            NAME_None,
            GetPlayersReach()
        );
    }
}

void UGrabber::Release()
{
    if (!PhysicsHandle) {return;}
    PhysicsHandle->ReleaseComponent();
}

// Checking for Physics Handle Component
void UGrabber::FindPhysicsHandle()
{
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (!PhysicsHandle) 
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
    FHitResult Hit;
    FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

    GetWorld()->LineTraceSingleByObjectType(
        Hit,
        GetPlayerViewPoint().Location,
        GetPlayersReach(),
        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
        TraceParams
    );

    return Hit;    
}

FPlayerViewPoint UGrabber::GetPlayerViewPoint() const
{
    FPlayerViewPoint PlayerViewPoint;

    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
            OUT PlayerViewPoint.Location,
            OUT PlayerViewPoint.Rotation
    );

    return PlayerViewPoint;
}

FVector UGrabber::GetPlayersReach() const
{
    FPlayerViewPoint PlayerViewPoint = GetPlayerViewPoint();
    return PlayerViewPoint.Location + PlayerViewPoint.Rotation.Vector() * Reach;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    if (!PhysicsHandle) {return;}
    if (PhysicsHandle->GrabbedComponent)
    {
        PhysicsHandle->SetTargetLocation(GetPlayersReach());
    }
}

