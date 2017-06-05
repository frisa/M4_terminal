#ifndef ELFMANAGER_H
#define ELFMANAGER_H

#include <QObject>
#include <QFile>
#include <QAbstractListModel>

#include "elf.h"


class ElfFile
{
public:
    ElfFile();
    ElfFile(const QString &name, const QString &abi);

public:
    QString m_name;
    QString m_sclass;
    QString m_dformat;
    QString m_abi;
    QString m_type;
    QString m_machine;
    int m_entry;
    int m_hsize;
    int m_pheader;
    int m_pheader_entries;
    int m_pheader_size;
    int m_sheader;
    int m_sheader_entries;
    int m_sheader_size;
    int m_flags;
};

class ElfListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ElfRoles {
        NameRole = Qt::UserRole + 1,
        SclassRole,
        DformatRole,
        AbiRole,
        TypeRole,
        MachineRole,
        EntryRole,
        HsizeRole,
        PheaderRole,
        PheaderEntriesRole,
        PheaderSizeRole,
        SheaderRole,
        SheaderEntriesRole,
        SheaderSizeRole,
        FlagsRole,
    };

    ElfListModel(QObject *parent = 0);

    void addFile(const ElfFile &file);
    void loadFile(QString sPath);
    void loadElfHeader(Elf32_Ehdr elf_header, ElfFile * elfFile);
    void loadElfSectionHeader(int32_t fd, Elf32_Ehdr eh, Elf32_Shdr sh_table[]);
    void loadSymbolTable(int32_t fd, Elf32_Shdr sh_table[], uint32_t symbol_table);
    void loadSymbols(int32_t fd, Elf32_Ehdr eh, Elf32_Shdr sh_table[]);

    void readElfHeader(int32_t fd, Elf32_Ehdr *elf_header);
    void readrSectionHeaderTable(int32_t fd, Elf32_Ehdr eh, Elf32_Shdr sh_table[]);
    char * readReadSection(int32_t fd, Elf32_Shdr sh);

    void saveTextSection(int32_t fd, Elf32_Ehdr eh, Elf32_Shdr sh_table[]);

    void addElfFile(QString sPath, ElfFile* elfFile);
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QList<ElfFile*> m_elfFiles;
};

#endif // ELFMANAGER_H
