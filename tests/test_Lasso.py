import pytest
import numpy as np
from plip.Lasso import *


@pytest.mark.parametrize(
    "num, output", [("test_data.forces", 4), ("test_data_w_b_l.forces", 6)]
)
def test_getNatoms(num, output):
    num_lines = getNatoms(num)
    assert num_lines == output


def test_readForces():
    input_file = "test_forces.txt"
    forces = np.array([[1.0, 2.0, 3.0], [4.0, 5.0, 6.0], [7.0, 8.0, 9.0]])
    np.savetxt(input_file, forces, header="force")

    # read forces
    result = readForces(input_file)

    assert result.all() == forces.all()

    input_file_with_nan = "test_forces_with_nan.txt"
    forces_with_nan = np.array(
        [[1.0, np.nan, 3.0], [4.0, 5.0, 6.0], [7.0, 8.0, np.nan]]
    )
    np.savetxt(input_file_with_nan, forces_with_nan)

    with pytest.raises(ValueError):
        readForces(input_file_with_nan)


def test_getBinaryFiles():
    input_file = "testdata/Refs/100.forces"
    type = "1"
    expected_result = ["poscar_f1_2B.bin", "poscar_f1_3B.bin", "poscar_f1_NB.bin"]
    result = getBinaryFiles(input_file, type)
    result = [string.replace("testdata/Input/100.", "") for string in result]
    assert result == expected_result


def test_readBinaryFiles():
    input_file = "testdata/Refs/100.forces"
    type = "1"
    list_bin = getBinaryFiles(input_file, type)

    r_xmat = readBinaryFiles(list_bin)
    print(f'results" {r_xmat}')
    expected_xmat = np.zeros((3 * 1, 3 * 2))  # 3*N, total_bin_files*Nc of bin file
    assert np.array_equal(r_xmat, expected_xmat)


def test_newreadFile():
    pass
