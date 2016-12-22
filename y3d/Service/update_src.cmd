if exist grpc (
REM xcopy .gitmodules grpc /D /Y
xcopy .gitignore grpc /D /Y
cd grpc
git pull origin master
git submodule update --recursive --remote
) else (
git clone --depth 1 https://github.com/grpc/grpc.git
REM xcopy .gitmodules grpc/ /D /Y
xcopy .gitignore grpc /Y
cd grpc
git submodule update --init --recursive --remote
REM git config core.sparseCheckout true
REM echo src/* >> .git/info/sparse-checkout
REM echo include/* >> .git/info/sparse-checkout
REM echo third_party/* >> .git/info/sparse-checkout
REM git checkout master
)

cd ..