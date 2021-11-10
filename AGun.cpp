

#include "AGun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AAGun::AAGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AAGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAGun::TriggerGun()
{
	//UE_LOG(Log,Warning,TEXT("Shoot"));
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash,Mesh,TEXT("MuzzleFlashSocket"));
	// viewpoint
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(!OwnerPawn) return;
	AController* OwnerController = OwnerPawn->GetController();
	if(!OwnerController) return;

	FVector Location; 
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location,Rotation);
	//Raycast
	FHitResult Hit;
	FVector End = Location + Rotation.Vector() * MaxRange;
	
	FCollisionQueryParams TraceParams(FName(TEXT("")),false,GetOwner());
	bool bSucess=GetWorld()->LineTraceSingleByChannel(Hit,Location,End,ECollisionChannel::ECC_GameTraceChannel1,TraceParams);
	if(bSucess)
	{
		FVector ShotDirection = -(Rotation.Vector());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ImpactFlash,Hit.Location,ShotDirection.Rotation());
		FPointDamageEvent DealDamage(DamageValue, Hit, ShotDirection, nullptr);
		if(Hit.GetActor() != nullptr)
		{
			Hit.GetActor()->TakeDamage(DamageValue, DealDamage, OwnerController, this);
		}
	}

}

