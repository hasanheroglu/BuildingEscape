// Copyright Hasan Husamettin Eroglu 2020.


#include "WorldPosition.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UWorldPosition::UWorldPosition()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWorldPosition::BeginPlay()
{
	Super::BeginPlay();

	FString OwnerName = GetOwner()->GetName();
    FString OwnerPosition = GetOwner()->GetActorLocation().ToString();
    
	UE_LOG(LogTemp, Warning, TEXT("Owner's name is %s"), *OwnerName);
    UE_LOG(LogTemp, Warning, TEXT("Owner is located at %s"), *OwnerPosition);
}


// Called every frame
void UWorldPosition::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

