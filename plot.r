library(ggplot2)

n = 3

data1000_0 <- read.table("./data/1000_0.txt", head=TRUE)
data2000_0 <- read.table("./data/2000_0.txt", head=TRUE)
data3000_0 <- read.table("./data/3000_0.txt", head=TRUE)
data4000_0 <- read.table("./data/4000_0.txt", head=TRUE)
data5000_0 <- read.table("./data/5000_0.txt", head=TRUE)

data1000_1 <- read.table("./data/1000_1.txt", head=TRUE)
data2000_1 <- read.table("./data/2000_1.txt", head=TRUE)
data3000_1 <- read.table("./data/3000_1.txt", head=TRUE)
data4000_1 <- read.table("./data/4000_1.txt", head=TRUE)
data5000_1 <- read.table("./data/5000_1.txt", head=TRUE)

data1000_2 <- read.table("./data/1000_2.txt", head=TRUE)
data2000_2 <- read.table("./data/2000_2.txt", head=TRUE)
data3000_2 <- read.table("./data/3000_2.txt", head=TRUE)
data4000_2 <- read.table("./data/4000_2.txt", head=TRUE)
data5000_2 <- read.table("./data/5000_2.txt", head=TRUE)

# Preprocessing time
pt_avg_0= array(1:n)
pt_std_0= array(1:n)
pt_avg_1= array(1:n)
pt_std_1= array(1:n)
pt_avg_2= array(1:n)
pt_std_2= array(1:n)

pt_avg_0[1] = mean(data1000_0$prepro_t)
pt_std_0[1] = sd(data1000_0$prepro_t)
pt_avg_1[1] = mean(data1000_1$prepro_t)
pt_std_1[1] = sd(data1000_1$prepro_t)
pt_avg_2[1] = mean(data1000_2$prepro_t)
pt_std_2[1] = sd(data1000_2$prepro_t)

pt_avg_0[2] = mean(data2000_0$prepro_t)
pt_std_0[2] = sd(data2000_0$prepro_t)
pt_avg_1[2] = mean(data2000_1$prepro_t)
pt_std_1[2] = sd(data2000_1$prepro_t)
pt_avg_2[2] = mean(data2000_2$prepro_t)
pt_std_2[2] = sd(data2000_2$prepro_t)

pt_avg_0[3] = mean(data3000_0$prepro_t)
pt_std_0[3] = sd(data3000_0$prepro_t)
pt_avg_1[3] = mean(data3000_1$prepro_t)
pt_std_1[3] = sd(data3000_1$prepro_t)
pt_avg_2[3] = mean(data3000_2$prepro_t)
pt_std_2[3] = sd(data3000_2$prepro_t)

pt_avg_0[4] = mean(data4000_0$prepro_t)
pt_std_0[4] = sd(data4000_0$prepro_t)
pt_avg_1[4] = mean(data4000_1$prepro_t)
pt_std_1[4] = sd(data4000_1$prepro_t)
pt_avg_2[4] = mean(data4000_2$prepro_t)
pt_std_2[4] = sd(data4000_2$prepro_t)

pt_avg_0[5] = mean(data5000_0$prepro_t)
pt_std_0[5] = sd(data5000_0$prepro_t)
pt_avg_1[5] = mean(data5000_1$prepro_t)
pt_std_1[5] = sd(data5000_1$prepro_t)
pt_avg_2[5] = mean(data5000_2$prepro_t)
pt_std_2[5] = sd(data5000_2$prepro_t)

# Preprocessing space
ps_avg_0= array(1:n)
ps_std_0= array(1:n)
ps_avg_1= array(1:n)
ps_std_1= array(1:n)
ps_avg_2= array(1:n)
ps_std_2= array(1:n)

ps_avg_0[1] = mean(data1000_0$prepro_s)
ps_std_0[1] = sd(data1000_0$prepro_s)
ps_avg_1[1] = mean(data1000_1$prepro_s)
ps_std_1[1] = sd(data1000_1$prepro_s)
ps_avg_2[1] = mean(data1000_2$prepro_s)
ps_std_2[1] = sd(data1000_2$prepro_s)

ps_avg_0[2] = mean(data2000_0$prepro_s)
ps_std_0[2] = sd(data2000_0$prepro_s)
ps_avg_1[2] = mean(data2000_1$prepro_s)
ps_std_1[2] = sd(data2000_1$prepro_s)
ps_avg_2[2] = mean(data2000_2$prepro_s)
ps_std_2[2] = sd(data2000_2$prepro_s)

ps_avg_0[3] = mean(data3000_0$prepro_s)
ps_std_0[3] = sd(data3000_0$prepro_s)
ps_avg_1[3] = mean(data3000_1$prepro_s)
ps_std_1[3] = sd(data3000_1$prepro_s)
ps_avg_2[3] = mean(data3000_2$prepro_s)
ps_std_2[3] = sd(data3000_2$prepro_s)

ps_avg_0[4] = mean(data4000_0$prepro_s)
ps_std_0[4] = sd(data4000_0$prepro_s)
ps_avg_1[4] = mean(data4000_1$prepro_s)
ps_std_1[4] = sd(data4000_1$prepro_s)
ps_avg_2[4] = mean(data4000_2$prepro_s)
ps_std_2[4] = sd(data4000_2$prepro_s)

ps_avg_0[5] = mean(data5000_0$prepro_s)
ps_std_0[5] = sd(data5000_0$prepro_s)
ps_avg_1[5] = mean(data5000_1$prepro_s)
ps_std_1[5] = sd(data5000_1$prepro_s)
ps_avg_2[5] = mean(data5000_2$prepro_s)
ps_std_2[5] = sd(data5000_2$prepro_s)

# Query time
qt_avg_0= array(1:n)
qt_std_0= array(1:n)
qt_avg_1= array(1:n)
qt_std_1= array(1:n)
qt_avg_2= array(1:n)
qt_std_2= array(1:n)

qt_avg_0[1] = mean(data1000_0$query_avg)
qt_std_0[1] = sd(data1000_0$query_avg)#sqrt(sum(data1000_0$query_std^2) / length(data1000_0$query_std))
qt_avg_1[1] = mean(data1000_1$query_avg)
qt_std_1[1] = sd(data1000_1$query_avg)#sqrt(sum(data1000_1$query_std^2) / length(data1000_1$query_std))
qt_avg_2[1] = mean(data1000_2$query_avg)
qt_std_2[1] = sd(data1000_2$query_avg)#sqrt(sum(data1000_2$query_std^2) / length(data1000_2$query_std))

qt_avg_0[2] = mean(data2000_0$query_avg)
qt_std_0[2] = sd(data2000_0$query_avg)#sqrt(sum(data2000_0$query_std^2) / length(data2000_0$query_std))
qt_avg_1[2] = mean(data2000_1$query_avg)
qt_std_1[2] = sd(data2000_1$query_avg)#sqrt(sum(data2000_1$query_std^2) / length(data2000_1$query_std))
qt_avg_2[2] = mean(data2000_2$query_avg)
qt_std_2[2] = sd(data2000_2$query_avg)#sqrt(sum(data2000_2$query_std^2) / length(data2000_2$query_std))

qt_avg_0[3] = mean(data3000_0$query_avg)
qt_std_0[3] = sd(data3000_0$query_avg)#sqrt(sum(data3000_0$query_std^2) / length(data3000_0$query_std))
qt_avg_1[3] = mean(data3000_1$query_avg)
qt_std_1[3] = sd(data3000_1$query_avg)#sqrt(sum(data3000_1$query_std^2) / length(data3000_1$query_std))
qt_avg_2[3] = mean(data3000_2$query_avg)
qt_std_2[3] = sd(data3000_2$query_avg)#sqrt(sum(data3000_2$query_std^2) / length(data3000_2$query_std))

qt_avg_0[4] = mean(data4000_0$query_avg)
qt_std_0[4] = sd(data4000_0$query_avg)#sqrt(sum(data4000_0$query_std^2) / length(data4000_0$query_std))
qt_avg_1[4] = mean(data4000_1$query_avg)
qt_std_1[4] = sd(data4000_1$query_avg)#sqrt(sum(data4000_1$query_std^2) / length(data4000_1$query_std))
qt_avg_2[4] = mean(data4000_2$query_avg)
qt_std_2[4] = sd(data4000_2$query_avg)#sqrt(sum(data4000_2$query_std^2) / length(data4000_2$query_std))

qt_avg_0[5] = mean(data5000_0$query_avg)
qt_std_0[5] = sd(data5000_0$query_avg)#sqrt(sum(data5000_0$query_std^2) / length(data5000_0$query_std))
qt_avg_1[5] = mean(data5000_1$query_avg)
qt_std_1[5] = sd(data5000_1$query_avg)#sqrt(sum(data5000_1$query_std^2) / length(data5000_1$query_std))
qt_avg_2[5] = mean(data5000_2$query_avg)
qt_std_2[5] = sd(data5000_2$query_avg)#sqrt(sum(data5000_2$query_std^2) / length(data5000_2$query_std))

# Query accuracy
qd_avg_0= array(1:n)
qd_std_0= array(1:n)
qd_avg_1= array(1:n)
qd_std_1= array(1:n)
qd_avg_2= array(1:n)
qd_std_2= array(1:n)

qd_avg_0[1] = mean(data1000_0$diff_avg)
qd_std_0[1] = sd(data1000_0$diff_avg)#sqrt(sum(data1000_0$diff_std^2) / length(data1000_0$diff_std))
qd_avg_1[1] = mean(data1000_1$diff_avg)
qd_std_1[1] = sd(data1000_1$diff_avg)#sqrt(sum(data1000_1$diff_std^2) / length(data1000_1$diff_std))
qd_avg_2[1] = mean(data1000_2$diff_avg)
qd_std_2[1] = sd(data1000_2$diff_avg)#sqrt(sum(data1000_2$diff_std^2) / length(data1000_2$diff_std))

qd_avg_0[2] = mean(data2000_0$diff_avg)
qd_std_0[2] = sd(data2000_0$diff_avg)#sqrt(sum(data2000_0$diff_std^2) / length(data2000_0$diff_std))
qd_avg_1[2] = mean(data2000_1$diff_avg)
qd_std_1[2] = sd(data2000_1$diff_avg)#sqrt(sum(data2000_1$diff_std^2) / length(data2000_1$diff_std))
qd_avg_2[2] = mean(data2000_2$diff_avg)
qd_std_2[2] = sd(data2000_2$diff_avg)#sqrt(sum(data2000_2$diff_std^2) / length(data2000_2$diff_std))

qd_avg_0[3] = mean(data3000_0$diff_avg)
qd_std_0[3] = sd(data3000_0$diff_avg)#sqrt(sum(data3000_0$diff_std^2) / length(data3000_0$diff_std))
qd_avg_1[3] = mean(data3000_1$diff_avg)
qd_std_1[3] = sd(data3000_1$diff_avg)#sqrt(sum(data3000_1$diff_std^2) / length(data3000_1$diff_std))
qd_avg_2[3] = mean(data3000_2$diff_avg)
qd_std_2[3] = sd(data3000_2$diff_avg)#sqrt(sum(data3000_2$diff_std^2) / length(data3000_2$diff_std))

qd_avg_0[4] = mean(data4000_0$diff_avg)
qd_std_0[4] = sd(data4000_0$diff_avg)#sqrt(sum(data4000_0$diff_std^2) / length(data4000_0$diff_std))
qd_avg_1[4] = mean(data4000_1$diff_avg)
qd_std_1[4] = sd(data4000_1$diff_avg)#sqrt(sum(data4000_1$diff_std^2) / length(data4000_1$diff_std))
qd_avg_2[4] = mean(data4000_2$diff_avg)
qd_std_2[4] = sd(data4000_2$diff_avg)#sqrt(sum(data4000_2$diff_std^2) / length(data4000_2$diff_std))

qd_avg_0[5] = mean(data5000_0$diff_avg)
qd_std_0[5] = sd(data5000_0$diff_avg)#sqrt(sum(data5000_0$diff_std^2) / length(data5000_0$diff_std))
qd_avg_1[5] = mean(data5000_1$diff_avg)
qd_std_1[5] = sd(data5000_1$diff_avg)#sqrt(sum(data5000_1$diff_std^2) / length(data5000_1$diff_std))
qd_avg_2[5] = mean(data5000_2$diff_avg)
qd_std_2[5] = sd(data5000_2$diff_avg)#sqrt(sum(data5000_2$diff_std^2) / length(data5000_2$diff_std))

# Plotting
x0 <- c(1000, 2000, 3000, 4000, 5000)
x <- x0 - 25
x2 <- x + 25
xf <- seq(1000, 5000, 100)

col0 = "purple"
col1 = "red"
col2 = "blue"

plot_pt = 1
plot_ps = 1
plot_qt = 1
plot_qd = 1

if (plot_pt == 1) {
    #pt_fit_0 <- nls(pt_avg_0 ~ a * x * log2(x)^3, data.frame(x, pt_avg_0), start=list(a=5000))
    pt_fit_0 <- nls(pt_avg_0 ~ a * x * log2(x)^3 + b, data.frame(x, pt_avg_0), start=list(a=5000, b=-1E9))
    #pt_fit_1 <- nls(pt_avg_1 ~ a * x * log2(x)^3, data.frame(x, pt_avg_1), start=list(a=5000))
    pt_fit_1 <- nls(pt_avg_1 ~ a * x * log2(x)^3 + b, data.frame(x, pt_avg_1), start=list(a=5000, b=-1E9))
    #pt_fit_2 <- nls(pt_avg_2 ~ a * x * log2(x)^3, data.frame(x, pt_avg_2), start=list(a=5000))
    pt_fit_2 <- nls(pt_avg_2 ~ a * x * log2(x)^3 + b, data.frame(x, pt_avg_2), start=list(a=5000, b=-1E9))

    a_0 <- coef(summary(pt_fit_0))["a", "Estimate"]
    b_0 <- coef(summary(pt_fit_0))["b", "Estimate"]
    a_1 <- coef(summary(pt_fit_1))["a", "Estimate"]
    b_1 <- coef(summary(pt_fit_1))["b", "Estimate"]
    a_2 <- coef(summary(pt_fit_2))["a", "Estimate"]
    b_2 <- coef(summary(pt_fit_2))["b", "Estimate"]

    d_0 <- data.frame(x = x0, y = pt_avg_0)
    df_0<- data.frame(x = xf, y = a_0*xf*log2(xf)^3+b_0)
    d_1 <- data.frame(x = x, y = pt_avg_1)
    df_1<- data.frame(x = xf, y = a_1*xf*log2(xf)^3+b_1)
    d_2 <- data.frame(x = x2, y = pt_avg_2)
    df_2<- data.frame(x = xf, y = a_2*xf*log2(xf)^3+b_2)

    ggplot(d_1, aes(x=x, y=y)) +
        scale_y_log10() +
        expand_limits(y=c(1E8, 1E10)) +
        geom_point(colour = col1) +
        geom_point(data = d_0, colour = col0) +
        geom_point(data = d_2, colour = col2) +
        geom_errorbar(aes(x=x, y=y, ymin=pt_avg_1-pt_std_1, ymax=pt_avg_1+pt_std_1), width=250, colour = col1) +
        geom_errorbar(data=d_0, aes(x=x, y=y, ymin=pt_avg_0-pt_std_0, ymax=pt_avg_0+pt_std_0), width=250, colour = col0) +
        geom_errorbar(data=d_2, aes(x=x, y=y, ymin=pt_avg_2-pt_std_2, ymax=pt_avg_2+pt_std_2), width=250, colour = col2) +
        geom_line(data=df_1, aes(x=x, y=y), colour = col1) +
        geom_line(data=df_0, aes(x=x, y=y), colour = col0) +
        geom_line(data=df_2, aes(x=x, y=y), colour = col2) +
        theme_bw() +
        xlab("Number of vertices in graph") +
        ylab("Run time in ns") +
        ggtitle("Preprocessing run time")
    ggsave("./figures/prepro_t.pdf")
}

if (plot_ps == 1) {
    ps_fit_0 <- nls(ps_avg_0 ~ a * x * log2(x) + b, data.frame(x, ps_avg_0), start=list(a=5, b=-1E4))
    ps_fit_1 <- nls(ps_avg_1 ~ a * x * log2(x) + b, data.frame(x, ps_avg_1), start=list(a=5, b=-1E4))
    ps_fit_2 <- nls(ps_avg_2 ~ a * x * log2(x) + b, data.frame(x, ps_avg_2), start=list(a=5, b=-1E4))

    a_0 <- coef(summary(ps_fit_0))["a", "Estimate"]
    b_0 <- coef(summary(ps_fit_0))["b", "Estimate"]
    a_1 <- coef(summary(ps_fit_1))["a", "Estimate"]
    b_1 <- coef(summary(ps_fit_1))["b", "Estimate"]
    a_2 <- coef(summary(ps_fit_2))["a", "Estimate"]
    b_2 <- coef(summary(ps_fit_2))["b", "Estimate"]

    d_0 <- data.frame(x = x0, y = ps_avg_0)
    df_0<- data.frame(x = xf, y = a_0*xf*log2(xf)+b_0)
    d_1 <- data.frame(x = x, y = ps_avg_1)
    df_1<- data.frame(x = xf, y = a_1*xf*log2(xf)+b_1)
    d_2 <- data.frame(x = x2, y = ps_avg_2)
    df_2<- data.frame(x = xf, y = a_2*xf*log2(xf)+b_2)

    ggplot(d_1, aes(x=x, y=y)) +
        scale_y_log10() +
        expand_limits(y=c(1E4, 1E6)) +
        geom_point(colour = col1) +
        geom_point(data = d_0, colour = col0) +
        geom_point(data = d_2, colour = col2) +
        geom_errorbar(aes(x=x, y=y, ymin=ps_avg_1-ps_std_1, ymax=ps_avg_1+ps_std_1), width=250, colour = col1) +
        geom_errorbar(data=d_0, aes(x=x0, y=y, ymin=ps_avg_0-ps_std_0, ymax=ps_avg_0+ps_std_0), width=250, colour = col0) +
        geom_errorbar(data=d_2, aes(x=x2, y=y, ymin=ps_avg_2-ps_std_2, ymax=ps_avg_2+ps_std_2), width=250, colour = col2) +
        geom_line(data=df_1, aes(x=x, y=y), colour = col1) +
        geom_line(data=df_0, aes(x=x, y=y), colour = col0) +
        geom_line(data=df_2, aes(x=x, y=y), colour = col2) +
        theme_bw() +
        xlab("Number of vertices in graph") +
        ylab("Number of 32 bit words used") +
        ggtitle("Preprocessing memory use")
    ggsave("./figures/prepro_s.pdf")
}

if (plot_qt == 1) {
    qt_fit_0 <- nls(qt_avg_0 ~ a * log2(x) + b, data.frame(x, qt_avg_0), start=list(a=5, b=-1E4))
    #qt_fit_0 <- nls(qt_avg_0 ~ a * log2(x), data.frame(x, qt_avg_0), start=list(a=5))
    qt_fit_1 <- nls(qt_avg_1 ~ a * log2(x) + b, data.frame(x, qt_avg_1), start=list(a=5, b=-1E4))
    #qt_fit_1 <- nls(qt_avg_1 ~ a * log2(x), data.frame(x, qt_avg_1), start=list(a=5))
    qt_fit_2 <- nls(qt_avg_2 ~ a * log2(x) + b, data.frame(x, qt_avg_2), start=list(a=5, b=-1E4))
    #qt_fit_2 <- nls(qt_avg_2 ~ a * log2(x), data.frame(x, qt_avg_2), start=list(a=5))

    a_0 <- coef(summary(qt_fit_0))["a", "Estimate"]
    b_0 <- coef(summary(qt_fit_0))["b", "Estimate"]
    a_1 <- coef(summary(qt_fit_1))["a", "Estimate"]
    b_1 <- coef(summary(qt_fit_1))["b", "Estimate"]
    a_2 <- coef(summary(qt_fit_2))["a", "Estimate"]
    b_2 <- coef(summary(qt_fit_2))["b", "Estimate"]

    d_0 <- data.frame(x = x0, y = qt_avg_0)
    df_0<- data.frame(x = xf, y = a_0*log2(xf)+b_0)
    #df_0<- data.frame(x = xf, y = a_0*log2(xf))
    d_1 <- data.frame(x = x, y = qt_avg_1)
    df_1<- data.frame(x = xf, y = a_1*log2(xf)+b_1)
    #df_1<- data.frame(x = xf, y = a_1*log2(xf))
    d_2 <- data.frame(x = x2, y = qt_avg_2)
    df_2<- data.frame(x = xf, y = a_2*log2(xf)+b_2)
    #df_2<- data.frame(x = xf, y = a_2*log2(xf))

    ggplot(d_1, aes(x=x, y=y)) +
        geom_point(colour = col1) +
        geom_point(data = d_0, colour = col0) +
        geom_point(data = d_2, colour = col2) +
        geom_errorbar(aes(x=x, y=y, ymin=qt_avg_1-qt_std_1, ymax=qt_avg_1+qt_std_1), width=250, colour = col1) +
        geom_errorbar(data=d_0, aes(x=x0, y=y, ymin=qt_avg_0-qt_std_0, ymax=qt_avg_0+qt_std_0), width=250, colour = col0) +
        geom_errorbar(data=d_2, aes(x=x2, y=y, ymin=qt_avg_2-qt_std_2, ymax=qt_avg_2+qt_std_2), width=250, colour = col2) +
        geom_line(data=df_1, aes(x=x, y=y), colour = col1) +
        geom_line(data=df_0, aes(x=x, y=y), colour = col0) +
        geom_line(data=df_2, aes(x=x, y=y), colour = col2) +
        theme_bw() +
        scale_y_continuous(limits=c(1E3, 5E3), breaks=c(1E3, 2E3, 3E3, 4E3, 5E3),
                           labels=c("1E3", "2E3", "3E3", "4E3", "5E3"), trans="log10") +
        xlab("Number of vertices in graph") +
        ylab("Run time in ns") +
        ggtitle("Querying run time")
    ggsave("./figures/query_t.pdf")
}

if (plot_qd == 1) {
    d_0 <- data.frame(x = x0, y = qd_avg_0)
    d_1 <- data.frame(x = x, y = qd_avg_1)
    d_2 <- data.frame(x = x2, y = qd_avg_2)

    ggplot(d_1, aes(x=x, y=y)) +
        geom_point(colour = col1) +
        geom_point(data = d_0, colour = col0) +
        geom_point(data = d_2, colour = col2) +
        geom_errorbar(aes(x=x, y=y, ymin=qd_avg_1-qd_std_1, ymax=qd_avg_1+qd_std_1), width=250, colour = col1) +
        geom_errorbar(data=d_0, aes(x=x0, y=y, ymin=qd_avg_0-qd_std_0, ymax=qd_avg_0+qd_std_0), width=250, colour = col0) +
        geom_errorbar(data=d_2, aes(x=x2, y=y, ymin=qd_avg_2-qd_std_2, ymax=qd_avg_2+qd_std_2), width=250, colour = col2) +
        expand_limits(y = c(0.8, 1.8)) +
        theme_bw() +
        xlab("Number of vertices in graph") +
        ylab("Oracle Distance / Dijkstra Distance") +
        ggtitle("Distance Oracle Accuracy")
    ggsave("./figures/query_d.pdf")
}

print("Done")
