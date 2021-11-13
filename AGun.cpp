//Header files.
#include "AGun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AAGun::AAGun()
{
 	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;
	//Attaching Gun object to this Script.
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

//Function resposible for shooting.
void AAGun::TriggerGun()
{
	//Spawn vfx.
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash,Mesh,TEXT("MuzzleFlashSocket"));
	
	//getting viewpoint
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(!OwnerPawn) return;
	AController* OwnerController = OwnerPawn->GetController();
	if(!OwnerController) return;
	FVector Location; 
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location,Rotation);
	
	//Raycasting
	FHitResult Hit;
	FVector End = Location + Rotation.Vector() * MaxRange;
	
	FCollisionQueryParams TraceParams(FName(TEXT("")),false,GetOwner());
	bool bSucess=GetWorld()->LineTraceSingleByChannel(Hit,Location,End,ECollisionChannel::ECC_GameTraceChannel1,TraceParams);
	
	//if raycasting hit an object
	if(bSucess)
	{	
		//shoot in the direction,
		//spawn vfx at the hit location.
		FVector ShotDirection = -(Rotation.Vector());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ImpactFlash,Hit.Location,ShotDirection.Rotation());
		FPointDamageEvent DealDamage(DamageValue, Hit, ShotDirection, nullptr);
		//if hit any Actor, decrease damage of that actor.
		if(Hit.GetActor() != nullptr)
		{
			Hit.GetActor()->TakeDamage(DamageValue, DealDamage, OwnerController, this);
		}
	}
}

