/*
MIT License

Copyright (c) 2022 Younes Meziane

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NetworkTimeSyncComponent.generated.h"

class UNetworkTimeSubsystem;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NETWORKTIMESYNC_API UNetworkTimeSyncComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UNetworkTimeSyncComponent();

protected:
	
	virtual void BeginPlay() override;

	/** Called on an interval to regularly synchronize network time with the server. */
	UFUNCTION()
	void SynchronizeNetworkTime();

	UFUNCTION(Server, Reliable)
	void Server_ReliableRequestWorldTime(const float ClientTimestamp);

	UFUNCTION(Server, Unreliable)
	void Server_UnreliableRequestWorldTime(const float ClientTimestamp);

	UFUNCTION(Client, Reliable)
	void Client_ReliableSendWorldTime(const float ClientTimestamp, const float ServerTimestamp);

	UFUNCTION(Client, Unreliable)
	void Client_UnreliableSendWorldTime(const float ClientTimestamp, const float ServerTimestamp);

	UPROPERTY()
	FTimerHandle Handle_SynchronizeNetworkTime;
	
	TWeakObjectPtr<UNetworkTimeSubsystem> NetworkTimeSubsystem;
	
	/** Whether or not to use unreliable RPCs when synchronizing network time. */
	bool bUseUnreliableRPCs = false;
};
