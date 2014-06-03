prependData <- read.csv("prepend_data.csv")

plot(prependData$prepends, prependData$rope, col="blue", pch=1, main="Number of Prepends vs Execution Time", xlab="Log Number of Prepends", ylab="Execution Time (seconds)", log="x")
lines(prependData$prepends, prependData$rope, col="blue", pch=1)

points(prependData[1:13,]$prepends, prependData[1:13,]$string, col="red", pch=2)
lines(prependData[1:13,]$prepends, prependData[1:13,]$string, col="red", pch=2)

legend('topleft', c("C++ String", "Rope"), col=c("red","blue"), pch=c(1,2))

dev.new()

appendData <- read.csv("append_data.csv")


plot(appendData$appends, appendData$rope, col="blue", pch=1, main="Number of Appends vs Execution Time", xlab="Log Number of Appends", ylab="Execution Time (seconds)", log="x")
lines(appendData$appends, appendData$rope, col="blue", pch=1)
points(appendData$appends, appendData$string, col="red", pch=2)
lines(appendData$appends, appendData$string, col="red", pch=2)

legend('topleft', c("Rope", "C++ String"), col=c("blue","red"), pch=c(1,2))

dev.new()


randomAccessData <- read.csv("random_access.csv")

plot(randomAccessData$size, randomAccessData$rope, col="blue", pch=1, main="Size of String vs Execution Time of Lookup", xlab="Log Size", ylab="Execution Time (seconds)", log="x")
lines(randomAccessData$size, randomAccessData$rope, col="blue", pch=1)
points(randomAccessData$size, randomAccessData$string, col="red", pch=2)
lines(randomAccessData$size, randomAccessData$string, col="red", pch=2)

legend('topleft', c("Rope", "C++ String"), col=c("blue","red"), pch=c(1,2))

dev.new()

substringData <- read.csv("random_access.csv")

plot(substringData$size, substringData$rope, col="blue", pch=1, main="Size of Substring vs Execution Time", xlab="Log Size", ylab="Execution Time (seconds)", log="x")
lines(substringData$size, substringData$rope, col="blue", pch=1)
points(substringData$size, substringData$string, col="red", pch=2)
lines(substringData$size, substringData$string, col="red", pch=2)

legend('topleft', c("Rope", "C++ String"), col=c("blue","red"), pch=c(1,2))


## plot(data$Size, data$D1, col="red", pch=1, main="Distance vs Domain Size", xlab="Domain Size", ylab="Tau Distance")
## legend(1500, 600, c("alpha = 1", "alpha = 5", "alpha = 10"), col=c("red","blue","green"), pch=c(1,2,3))
## curve(-4.77 + 0.1821 * x * log(x), 1, 2000 , add=TRUE)
## abline(-125, 1.42, col="grey")

## dev.new()

## varianceData <- read.csv("data3.csv")
## plot(varianceData$Time, varianceData$Distance, ylim = c(0,1500), main="Distance vs Time for fixed Domain", xlab="Time Steps", ylab="Tau Distance")
## abline(h=quantile(varianceData$Distance,c(0.25,0.5,0.75)))

## dev.new()

## timeVariance <- read.csv("data4.csv")
## plot(timeVariance$Time, ylim = c(5000,13000), main="Number of Comparisons per Iteration", xlab="Iteration", ylab="Number of Comparisons")
## abline(h=quantile(timeVariance$Time,c(0.25,0.5,0.75)))

## dev.new()

## timeData <- read.csv("data5.csv")
## plot(timeData$Size, timeData$Time, main="Average Number of Comparisons vs Domain Size", xlab="Domain Size", ylab="Average Number of Comparisons")
## curve(-100 + 1.62 * x * log(x), 1, 2000, add=TRUE)
## segments(timeData$Size, timeData$Time - timeData$Stddev, timeData$Size, timeData$Time + timeData$Stddev)
## abline(-1464, 12.8, col="grey")

## epsilon <- 5

## segments(timeData$Size - epsilon, timeData$Time - timeData$Stddev, timeData$Size + epsilon, timeData$Time - timeData$Stddev)

## segments(timeData$Size - epsilon, timeData$Time + timeData$Stddev, timeData$Size + epsilon, timeData$Time + timeData$Stddev)


