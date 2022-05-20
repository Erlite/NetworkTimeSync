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
#include "Subsystems/GameInstanceSubsystem.h"
#include "NetworkTimeSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNetworkClockSynchronized, float, OldServerDelta, float, NewServerDelta);

/**
 * Subsystem used to retrieve a more accurate server world time.
 */
UCLASS()
class NETWORKTIMESYNC_API UNetworkTimeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	/**
	 * Retrieve the network time subsystem from a world context object.
	 * @param WorldContextObject A valid world context object to retrieve the subsystem from.
	 */
	static UNetworkTimeSubsystem* Get(UObject* WorldContextObject);
	
	/** Returns the synchronized time between the server and client. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Network Time Subsystem")
	float GetServerWorldTime() const;

	/** Returns the shortest round trip time recorded upon synchronization. Clientside only. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Network Time Subsystem")
	FORCEINLINE float GetShortestRoundTripTime() const { return ShortestRoundTripTime; }

	/** Returns the delta time between the server's world time seconds and the client's. Clientside only. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Network Time Subsystem")
	FORCEINLINE float GetServerWorldTimeDelta() const { return ServerWorldTimeDelta; }

	/**
	 * Delegate fired on clients when synchronizing the clock.
	 * Only called if accuracy increased.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Network Time Subsystem")
	FOnNetworkClockSynchronized OnNetworkClockSynchronized;

protected:

	friend class UNetworkTimeSyncComponent;
	
	/** Called by the UNetworkTimeSyncComponent when it receives an updated world time from the server. */
	void OnServerWorldTimeReceived(const float ClientTime, const float ServerTime);

	/**
	 * The shortest round trip time recorded upon synchronization.
	 * Clientside only.
	 */
	float ShortestRoundTripTime = TNumericLimits<float>::Max();

	/**
	 * The delta time between the server's world time seconds and the client's.
	 * Clientside only.
	 */
	float ServerWorldTimeDelta = 0.0f;
};
